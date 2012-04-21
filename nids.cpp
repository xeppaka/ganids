#include <fstream>
#include <sstream>
#include <string>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <cuda_runtime_api.h>

#include "common.h"
#include "nids.h"
#include "my_nids_cuda.h"

#include <iostream>
#include <iomanip>

using namespace std;

using std::string;
using std::ifstream;

#define CUDA_TASK_SIZE 4

void packet_received(u_char *user,
                     const struct pcap_pkthdr *h,
                     const u_char *bytes)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - packet_received";
    Nids *nids = reinterpret_cast<Nids *>(user);

    ++nids->num_packets;
    nids->num_bytes += h->caplen;

    Packet *packet = Packet::create_packet(bytes);

    if (packet) {
        ++nids->num_tcp_ip_packets;
        nids->num_tcp_ip_payload_bytes += packet->get_payload_size();

        if (nids->gpu_enabled) {
            nids->process_packet_gpu(packet);
            BOOST_LOG_TRIVIAL(trace) << "packet buffer size: " << nids->packet_buffer->size();
        }
        else {
            packet->save_payload();
            nids->packets_queue.push(packet);
            nids->packets_queue_sem.post();
        }
    }

    BOOST_LOG_TRIVIAL(trace) << "End - packet_received";
}

Nids::Nids() :
    handle(NULL),
    state(IDLE),
    num_rules(0),
    num_packets(0),
    num_tcp_ip_packets(0),
    num_tcp_ip_payload_bytes(0),
    cur_buffers(0),
    gpu_task_ready(0),
    gpu_task_finished(1),
    gpu_init_finished(0),
    packets_queue_sem(0),
    threads_finished_sem(0)
{
    cur_buffers = 1;
    switch_buffers();
}

Nids::~Nids()
{
    free_rules();
    if (handle)
        stop_monitor();
}

int Nids::read_rules(const char *filename)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::read_rules";
    ifstream f;
    f.open(filename);
    if (!f.good()) {
        BOOST_LOG_TRIVIAL(error) << "Unable to read file: " << filename;
        BOOST_LOG_TRIVIAL(trace) << "End - Nids::read_rules";
        return -1;
    }

    rules_trie.clear();
    free_rules();

    string line;
    while (!f.eof()) {
        std::getline(f, line);
        if (line.size() <= 0)
            continue;

        Rule *new_rule = Rule::create_rule(line);

        if (new_rule) {
            rules.push_back(new_rule);
            rules_trie.insert(new_rule->get_route(), rules.size() - 1);

            const vector<sequence8> &content = new_rule->get_content();
            for (vector<sequence8>::const_iterator it = content.begin(); it != content.end(); ++it)
                ac.insert(*it, rules.size() - 1);

            const sequence8 &pcre = new_rule->get_pcre();
            if (pcre.size() > 0)
                regex_dfa_offset.push_back(regex.compile(pcre));
            else
                regex_dfa_offset.push_back(-1);
        }
    }

    ac.finish();

    BOOST_LOG_TRIVIAL(info) << "number of loaded rules: " << rules.size();
    BOOST_LOG_TRIVIAL(trace) << "End - Nids::read_rules";
    return rules.size();
}

bool Nids::start_monitor(const char *interface, int threads_num, const char *db_filename)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::start_monitor";

    num_packets = 0;
    num_tcp_ip_packets = 0;
    num_bytes = 0;
    num_tcp_ip_payload_bytes = 0;

    if (db_filename)
        db.open(db_filename);

    char errbuf[PCAP_ERRBUF_SIZE];

    // open pcap session
    handle = pcap_open_live(interface, 65535, 1, 0, errbuf);
    if (handle == NULL) {
        BOOST_LOG_TRIVIAL(error) << "pcap_open_live() failed for interface '" << interface << "'";
        BOOST_LOG_TRIVIAL(trace) << "End - Nids::start_monitor";
        return false;
    } else {
        BOOST_LOG_TRIVIAL(debug) << "pcap_open_live() success";
    }

    gpu_enabled = false;

    if (db_filename)
        db.open(db_filename);

    threads_exit = false;

    cpu_exec_threads = new boost::thread*[threads_num > 0 ? threads_num : 1];
    for (int i = 0; i < threads_num; ++i)
        cpu_exec_threads[i] = new boost::thread(process_on_cpu_callable(), this);

    int res = pcap_loop(handle, 0, packet_received, reinterpret_cast<u_char *>(this));
    BOOST_LOG_TRIVIAL(debug) << "pcap_loop() returned: " << res;

    threads_exit = true;

    for (int i = 0; i < threads_num * 10; ++i)
        packets_queue_sem.post();

    // here we should wait until all capture threads return
    for (int i = 0; i < threads_num; ++i)
        threads_finished_sem.timed_wait(boost::get_system_time() + boost::posix_time::milliseconds(500));

    if (handle) {
        pcap_close(handle);
        handle = NULL;
        BOOST_LOG_TRIVIAL(trace) << "pcap handle closed";
    }

    for (int i = 0; i < threads_num; ++i)
        delete cpu_exec_threads[i];
    delete [] cpu_exec_threads;

//    while (true) {
//        Packet *p = Packet::create_fake_packet();
//        p->save_payload();
//        packets_queue.push(p);
//        packets_queue_sem.post();
//        sleep(200);
//    }

    state = IDLE;

    BOOST_LOG_TRIVIAL(trace) << "End - Nids::start_monitor";
    return true;
}

bool Nids::start_monitor_gpu(const char *interface, int device_num, WINDOW_TYPE window_type, int buf_threshold_size, int buf_flush_time, const char *db_filename)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::start_monitor_gpu";

    this->window_type = window_type;
    buffer_threshold = buf_threshold_size;
    buffer_flush_time = buf_flush_time;

    char errbuf[PCAP_ERRBUF_SIZE];

    gpu_enabled = true;

    boost::thread th(process_on_gpu_callable(), this, device_num);

    gpu_init_finished.wait();
    if (!cuda_init_ok)
        return false;

    if (db_filename)
        db.open(db_filename);

    threads_exit = false;

    // open pcap session
    handle = pcap_open_live(interface, 65535, 1, 0, errbuf);
    if (handle == NULL) {
        BOOST_LOG_TRIVIAL(error) << "pcap_open_live() failed for interface '" << interface << "'";
        BOOST_LOG_TRIVIAL(trace) << "End - Nids::start_monitor";
        return false;
    } else {
        BOOST_LOG_TRIVIAL(debug) << "pcap_open_live() success";
    }

    int res = pcap_loop(handle, 0, packet_received, reinterpret_cast<u_char *>(this));
    BOOST_LOG_TRIVIAL(debug) << "pcap_loop() returned: " << res;

    //
    threads_exit = true;
    gpu_task_ready.post();

    // here we should wait until gpu process thread return
    threads_finished_sem.timed_wait(boost::get_system_time() + boost::posix_time::milliseconds(500));

    if (handle) {
        pcap_close(handle);
        handle = NULL;
        BOOST_LOG_TRIVIAL(trace) << "pcap handle closed";
    }

//    while (true) {
//        Packet *packet = Packet::create_fake_packet();
//        process_packet_gpu(packet);
//    }
    BOOST_LOG_TRIVIAL(trace) << "End - Nids::start_monitor_gpu";
}

bool Nids::list_cuda_devices(vector<cudaDeviceProp> &device_properties)
{
    int count;
    if (cudaGetDeviceCount(&count) != cudaSuccess)
        return false;

    cudaDeviceProp prop;
    for (int i = 0; i < count; ++i) {
        if (cudaGetDeviceProperties(&prop, i) == cudaSuccess)
            device_properties.push_back(prop);
    }

    return true;
}

bool Nids::stop_monitor()
{
    pcap_breakloop(handle);
}

bool Nids::list_interfaces(vector<string> &result)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::list_interfaces";
    pcap_if_t *devs;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&devs, errbuf) == -1) {
        BOOST_LOG_TRIVIAL(debug) << "pcap_findalldevs() failed";
        BOOST_LOG_TRIVIAL(trace) << "End - Nids::list_interfaces";
        return false;
    }

    result.clear();

    pcap_if_t *cur_dev = devs;
    while (cur_dev != NULL) {
        result.push_back(string(cur_dev->name));
        cur_dev = cur_dev->next;
    }

    BOOST_LOG_TRIVIAL(trace) << "End - Nids::list_interfaces";
    return true;
}

int Nids::packet_buffer_size() const
{
    return packet_buffer->size();
}

void Nids::analyze_header(const Packet *packet, vector<int> &result)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::analyze_header";
    vector<int> res;
    rules_trie.search(packet->get_route(), res);
    for (vector<int>::iterator it = res.begin(); it != res.end(); ++it)
        result.push_back(*it);

    BOOST_LOG_TRIVIAL(trace) << "End - Nids::analyze_header";
}

void Nids::analyze_payload_ac(const Packet *packet, vector<int> &result)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::analyze_header";
    ac.search(packet->get_payload().data(), packet->get_payload_size(), result);
    BOOST_LOG_TRIVIAL(trace) << "End - Nids::analyze_header";
}

void Nids::free_rules()
{
    for (vector<Rule *>::iterator it = rules.begin(); it != rules.end(); ++it)
        delete *it;
}

bool Nids::cuda_buffer_reallocate(u_char **buffer_pointer, int size)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::cuda_buffer_reallocate";

    if (*buffer_pointer != NULL)
        if (cudaFree(*buffer_pointer) != cudaSuccess) {
            **buffer_pointer = NULL;
            BOOST_LOG_TRIVIAL(trace) << "End - Nids::cuda_buffer_reallocate";
            return false;
        }

    *buffer_pointer = NULL;
    if (cudaMalloc(reinterpret_cast<void**>(buffer_pointer), size) != cudaSuccess) {
        BOOST_LOG_TRIVIAL(trace) << "End - Nids::cuda_buffer_reallocate";
        return false;
    }

    BOOST_LOG_TRIVIAL(trace) << "End - Nids::cuda_buffer_reallocate";
    return true;
}

bool Nids::buffer_reallocate(u_char **buffer_pointer, int size)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::buffer_reallocate";

    if (*buffer_pointer != NULL)
        delete *buffer_pointer;

    *buffer_pointer = new u_char[size];

    BOOST_LOG_TRIVIAL(trace) << "End - Nids::buffer_reallocate";
    return true;
}

bool Nids::analyze_payload_regex(const Packet *packet, int dfa_offset)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::analyze_payload_regex";
    bool res = regex.search(packet->get_payload().data(), packet->get_payload_size(), dfa_offset);
    BOOST_LOG_TRIVIAL(trace) << "End - Nids::analyze_payload_regex";
    return res;
}

void Nids::process_packet_gpu(Packet *packet)
{
    BOOST_LOG_TRIVIAL(trace) << "Begin - Nids::process_packet_gpu";

    matched_rules.clear();
    analyze_header(packet, matched_rules);

    // Check if some of headers rules matched.
    // If it's matched we need to analyze payload.
    if (matched_rules.size() > 0) {
        bool ac_analyze_needed = false;

        // locked in gpu_switch_buffer_mutex
        {
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(gpu_switch_buffer_mutex);
            for (vector<int>::iterator it = matched_rules.begin(); it != matched_rules.end(); ++it) {
                Rule *r = rules[*it];
                if (regex_dfa_offset[*it] != -1) {
                    tasks->push_back(packet_buffer->size());
                    tasks->push_back(packet->get_payload_size());
                    tasks->push_back(regex_dfa_offset[*it] + ac.count_states());
                    tasks->push_back(*it);
                }

                if (r->get_content().size() > 0) {
                    ac_analyze_needed = true;
                    ac_rules->push_back(*it);
                }
            }

            if (ac_analyze_needed) {
                tasks->push_back(packet_buffer->size());
                tasks->push_back(packet->get_payload_size());
                tasks->push_back(0);
                tasks->push_back(0);
            }

            packet_buffer->insert(packet_buffer->end(),
                                 packet->get_raw_packet() + packet->get_payload_offset(),
                                 packet->get_raw_packet() + packet->get_payload_offset() + packet->get_payload_size());

//            for (int i = 0; i < packet->get_payload_size(); ++i) {
//                cout << (packet->get_raw_packet() + packet->get_payload_offset())[i] << ' ';
//            }

//            cout << endl;
        }

        if (window_type == BUF_SIZE && packet_buffer->size() >= buffer_threshold && gpu_task_finished.try_wait())
            gpu_task_ready.post();
    }

    BOOST_LOG_TRIVIAL(trace) << "End - Nids::process_packet_gpu";
}

void Nids::process_result()
{
    boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(analyzing_result_mutex);
    for (vector<int>::iterator it = analyzing_result.begin();it != analyzing_result.end(); ++it)
        cout << *it << endl;
    analyzing_result.clear();
}

void Nids::switch_buffers()
{
    boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(gpu_switch_buffer_mutex);
    if (cur_buffers == 0) {
        tasks = &tasks2;
        packet_buffer = &packet_buffer2;
        ac_rules = &ac_rules2;

        gpu_tasks = &tasks1;
        gpu_packet_buffer = &packet_buffer1;
        gpu_ac_rules = &ac_rules1;

        cur_buffers = 1;
    } else {
        tasks = &tasks1;
        packet_buffer = &packet_buffer1;
        ac_rules = &ac_rules1;

        gpu_tasks = &tasks2;
        gpu_packet_buffer = &packet_buffer2;
        gpu_ac_rules = &ac_rules2;

        cur_buffers = 0;
    }
}

// thread which executes packet analyzing on GPU
void process_on_gpu_callable::operator ()(Nids *nids, int device_num)
{
    int *dfa_device = NULL;
    size_t pitch;

    try {
        // initialize CUDA device and copy DFAs
        // we need to do it here, because CUDA device should be initialized
        // in the thread where it will be used

        // set cuda device
        if (cudaSetDevice(device_num) != cudaSuccess)
            throw 1;

        // all states include regex states + ac states
        int states_all = nids->ac.count_states() + nids->regex.count_states();

        // alloc buffer for DFA
        if (cudaMallocPitch(reinterpret_cast<void**>(&dfa_device), &pitch, (STATE_SIZE + 1) * sizeof(dfa_device[0]), states_all) != cudaSuccess)
            throw 1;

        // copy AC DFA to cuda device
        for (int i = 0; i < nids->ac.count_states(); ++i) {
            int state_offset = i * pitch / sizeof(dfa_device[0]);
            if (cudaMemcpy(dfa_device + state_offset,
                   nids->ac.get_transition_row(i).data(),
                   STATE_SIZE * sizeof(dfa_device[0]), cudaMemcpyHostToDevice) != cudaSuccess)
                throw 1;
            int has_out = nids->ac.get_out(i).size() > 0 ? 1 : 0;
            if (cudaMemcpy(dfa_device + state_offset + STATE_SIZE, &has_out, sizeof(has_out), cudaMemcpyHostToDevice) != cudaSuccess)
                throw 1;
        }

        // copy all REGEX dfa's to cuda device
        int ac_size = nids->ac.count_states() * pitch / sizeof(dfa_device[0]);
        for (int i = 0; i < nids->regex.count_states(); ++i) {
            int state_offset = i * pitch / sizeof(dfa_device[0]) + ac_size;
            if (cudaMemcpy(dfa_device + state_offset,
                   nids->regex.get_transition_state(i).data(),
                   (STATE_SIZE + 1) * sizeof(dfa_device[0]), cudaMemcpyHostToDevice) != cudaSuccess)
                throw 1;
        }
    } catch (int i) {
        if (dfa_device)
            cudaFree(dfa_device);
        nids->cuda_init_ok = false;
        return;
    }

    u_char *cuda_packet_buffer = NULL;
    int *cuda_tasks_buffer = NULL;
    int *cuda_out_buffer = NULL;
    int *out_buffer = NULL;
    int cuda_packet_buffer_size = 0;
    int cuda_tasks_buffer_size = 0;
    int cuda_out_buffer_size = 0;
    int out_buffer_size = 0;

    nids->cuda_init_ok = true;
    nids->gpu_init_finished.post();

    try {
        while (true) {
            if (nids->window_type == BUF_SIZE)
                nids->gpu_task_ready.wait();
            else
                nids->gpu_task_ready.timed_wait(boost::get_system_time() + boost::posix_time::milliseconds(nids->buffer_flush_time));

            if (nids->threads_exit)
                break;

            nids->switch_buffers();

            //BOOST_LOG_TRIVIAL(trace) << "processing buffer of packets on GPU, length = " << nids->packet_buffer->size();
            cout << "processing buffer of packets on GPU, length = " << nids->gpu_packet_buffer->size() << endl;

            try {
                if (cuda_packet_buffer_size < nids->gpu_packet_buffer->size()) {
                    cuda_packet_buffer_size = nids->gpu_packet_buffer->size();
                    if (!nids->cuda_buffer_reallocate(&cuda_packet_buffer, cuda_packet_buffer_size))
                        throw 1;
                }

                if (cuda_tasks_buffer_size < nids->gpu_tasks->size() * sizeof(int)) {
                    cuda_tasks_buffer_size = nids->gpu_tasks->size() * sizeof(int);
                    if (!nids->cuda_buffer_reallocate(reinterpret_cast<u_char**>(&cuda_tasks_buffer), cuda_tasks_buffer_size))
                        throw 1;
                }

                if (cuda_out_buffer_size < nids->gpu_tasks->size() / CUDA_TASK_SIZE * sizeof(int)) {
                    cuda_out_buffer_size = nids->gpu_tasks->size() / CUDA_TASK_SIZE * sizeof(int);
                    if (!nids->cuda_buffer_reallocate(reinterpret_cast<u_char**>(&cuda_out_buffer), cuda_out_buffer_size))
                        throw 1;
                }

                // copy packets into CUDA device
                if (cudaMemcpy(cuda_packet_buffer, nids->gpu_packet_buffer->data(), nids->gpu_packet_buffer->size(), cudaMemcpyHostToDevice) != cudaSuccess)
                    throw 1;
                // copy tasks (pointers to DFA, lengths of packets)
                if (cudaMemcpy(cuda_tasks_buffer, nids->gpu_tasks->data(), nids->gpu_tasks->size() * sizeof(int), cudaMemcpyHostToDevice) != cudaSuccess)
                    throw 1;

                analyze_payload_cuda(dfa_device, pitch, cuda_packet_buffer, cuda_tasks_buffer, cuda_out_buffer, nids->gpu_tasks->size() / 4);

                if (out_buffer_size < nids->gpu_tasks->size() / CUDA_TASK_SIZE * sizeof(int)) {
                    out_buffer_size = nids->gpu_tasks->size() / CUDA_TASK_SIZE * sizeof(int);
                    nids->buffer_reallocate(reinterpret_cast<u_char**>(&out_buffer), out_buffer_size);
                }

                if (cudaMemcpy(out_buffer, cuda_out_buffer, nids->gpu_tasks->size() / CUDA_TASK_SIZE * sizeof(int), cudaMemcpyDeviceToHost) != cudaSuccess)
                    throw 1;
            } catch (int i) {
                BOOST_LOG_TRIVIAL(debug) << "error occured during processing packets on GPU";
                nids->gpu_packet_buffer->clear();
                nids->gpu_tasks->clear();
                nids->gpu_ac_rules->clear();

                if (cuda_packet_buffer)
                    cudaFree(cuda_packet_buffer);

                if (cuda_tasks_buffer)
                    cudaFree(cuda_packet_buffer);

                if (cuda_out_buffer)
                    cudaFree(cuda_packet_buffer);

                if (out_buffer)
                    delete [] out_buffer;

                if (dfa_device)
                    cudaFree(dfa_device);

                BOOST_LOG_TRIVIAL(trace) << "End - Nids::process_packet_gpu";
                return;
            }

            // process output
            bool found = false;
            for (int i = 0; i < nids->gpu_tasks->size() / 4; ++i) {
                bool ac_search = nids->gpu_tasks->at(i * 4 + 2) == 0;
                if (out_buffer[i] == -1)
                    continue;
                if (ac_search) {
                    // current task is AC search
                    const vector<int> &ac_out_rules = nids->ac.get_out(out_buffer[i]);
                    for (vector<int>::const_iterator it = ac_out_rules.begin(); it != ac_out_rules.end(); ++it) {
                        if (find(nids->gpu_ac_rules->begin(), nids->gpu_ac_rules->end(), *it) != nids->gpu_ac_rules->end()) {
                            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(nids->analyzing_result_mutex);
                            nids->analyzing_result.push_back(*it);
                            found = true;
                        }
                    }
                } else {
                    // current task is REGEX search
                    boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(nids->analyzing_result_mutex);
                    nids->analyzing_result.push_back(out_buffer[i]);
                    found = true;
                }
            }

            nids->gpu_packet_buffer->clear();
            nids->gpu_tasks->clear();
            nids->gpu_ac_rules->clear();

            if (found)
                nids->process_result();

            nids->gpu_task_finished.post();
        }
    } catch (boost::thread_interrupted e)
    {
        cout << "thread interrupted" << endl;
    }

    if (cuda_packet_buffer)
        cudaFree(cuda_packet_buffer);

    if (cuda_tasks_buffer)
        cudaFree(cuda_packet_buffer);

    if (cuda_out_buffer)
        cudaFree(cuda_packet_buffer);

    if (out_buffer)
        delete [] out_buffer;

    if (dfa_device)
        cudaFree(dfa_device);

    nids->threads_finished_sem.post();
    BOOST_LOG_TRIVIAL(trace) << "capture thread finished successfully" << endl;
}

void process_on_cpu_callable::operator ()(Nids *nids)
{
    vector<int> ac_rules;
    vector<int> matched_rules;
    Packet *packet;
    try {
        while (true) {
            nids->packets_queue_sem.wait();

            if (nids->threads_exit)
                break;

            {
                boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(nids->packets_queue_mutex);
                if (nids->packets_queue.size() <= 0)
                    continue;

                packet = nids->packets_queue.front();
                nids->packets_queue.pop();
            }

            bool found = false;

            matched_rules.clear();
            nids->analyze_header(packet, matched_rules);
            if (matched_rules.size() > 0) {
                bool ac_analyze_needed = false;

                ac_rules.clear();
                for (vector<int>::iterator it = matched_rules.begin(); it != matched_rules.end(); ++it) {
                    Rule *r = nids->rules[*it];
                    if (r->get_content().size() > 0) {
                        ac_analyze_needed = true;
                        ac_rules.push_back(*it);
                    }

                    if (nids->regex_dfa_offset[*it] != -1) {
                        if (nids->analyze_payload_regex(packet, nids->regex_dfa_offset[*it])) {
                            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(nids->analyzing_result_mutex);
                            nids->analyzing_result.push_back(*it);
                            found = true;
                        }
                    }
                }

                if (ac_analyze_needed) {
                    vector<int> result;
                    nids->analyze_payload_ac(packet, result);
                    if (result.size() > 0) {
                        for (vector<int>::iterator it = result.begin(); it != result.end(); ++it) {
                            if (find(ac_rules.begin(), ac_rules.end(), *it) != ac_rules.end()) {
                                boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(nids->analyzing_result_mutex);
                                nids->analyzing_result.push_back(*it);
                                found = true;
                            }
                        }
                    }
                }
            }

            if (found)
                nids->process_result();
        }
    } catch (boost::thread_interrupted e) {
        BOOST_LOG_TRIVIAL(info) << "thread interrupted" << endl;
    }

    nids->threads_finished_sem.post();
    BOOST_LOG_TRIVIAL(trace) << "capture thread finished successfully" << endl;
}

void Nids::set_log_level(LOG_LEVEL level)
{
//    logging::init_log_to_file
//        (
//            "my_nids.log",
//            keywords::filter = flt::attr< logging::trivial::severity_level >("Severity") >= logging::trivial::trace,
//            keywords::auto_flush = true
//        );

    logging::trivial::severity_level slevel;

    switch (level) {
    case TRACE:
        slevel = logging::trivial::trace;
        break;
    case INFO:
        slevel = logging::trivial::info;
        break;
    case DEBUG:
        slevel = logging::trivial::debug;
        break;
    case WARNING:
        slevel = logging::trivial::warning;
        break;
    case ERROR:
        slevel = logging::trivial::error;
        break;
    case FATAL:
        slevel = logging::trivial::fatal;
        break;
    }

    logging::core::get()->set_filter
       (
           flt::attr< logging::trivial::severity_level >("Severity") >= slevel
       );
}
