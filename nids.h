#ifndef NIDS_H
#define NIDS_H

#include <vector>
#include <string>
#include <queue>
#include <boost/thread/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include <cuda.h>
#include <cuda_runtime_api.h>

#include <pcap.h>

#include "trie.h"
#include "rule.h"
#include "packet.h"
#include "ac.h"
#include "regex.h"
#include "db.h"

using std::string;
using std::vector;
using std::queue;

#define DEF_PACKET_BUFFER_SIZE 65536
#define DEF_TIME_FLUSH_BUFFER 1000

enum LOG_LEVEL {
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

enum NIDS_STATE {
    IDLE = 0,
    RUNNING
};

enum WINDOW_TYPE {
    BUF_SIZE = 0,
    TIME
};

class Nids;

struct process_on_gpu_callable {
    void operator() (Nids* nids, int device_num);
};

struct process_on_cpu_callable {
    void operator() (Nids* nids);
};

class Nids
{
public:
    Nids();
    ~Nids();

    /**
      * \brief Function used to read rules from *.txt file and save them in Nids class in internal structures
      */
    int read_rules(const char *filename);
    bool start_monitor(const char *interface, int threads_num, const char *db_filename = NULL);
    bool stop_monitor();
    bool list_interfaces(vector<string> &result);
    int packet_buffer_size() const;
    bool start_monitor_gpu(const char *interface,
                           int device_num,
                           WINDOW_TYPE window_type = BUF_SIZE,
                           int buf_threshold_size = DEF_PACKET_BUFFER_SIZE,
                           int buf_flush_time = DEF_TIME_FLUSH_BUFFER,
                           const char *db_filename = NULL);
    bool list_cuda_devices(vector<cudaDeviceProp> &device_properties);
    void set_log_level(LOG_LEVEL level);

//private:
public:
    void analyze_header(const Packet *packet, vector<int> &result);
    void analyze_payload_ac(const Packet *packet, vector<int> &result);
    bool analyze_payload_regex(const Packet *packet, int dfa_offset);

    void process_packet(Packet *packet);
    void process_packet_gpu(Packet *packet);

    void process_result();

    void free_rules();
    bool cuda_buffer_reallocate(u_char **buffer_pointer, int size);
    bool buffer_reallocate(u_char **buffer_pointer, int size);

    void analyze_payload_cuda_emulate(int *dfa,
                                     int pitch,
                                     unsigned char *packet_buffer,
                                     int *task_buffer,
                                     int *out_buffer,
                                     int tasks);

    // object for database manipulations
    Db db;

    // pcap handle
    pcap_t *handle;

    // loaded rules
    vector<Rule *> rules;

    // route info saved in trie
    Trie rules_trie;

    // Aho-Corasick saved strings
    AC ac;

    // PCRE implementation with DFA extraction support
    Regex regex;

    // NIDS state
    NIDS_STATE state;

    // is GPU processing enabled?
    bool gpu_enabled;

    vector<int> analyzing_result;
    vector<int> regex_dfa_offset;

    // CPU processing
    boost::thread **cpu_exec_threads;
    queue<Packet *> packets_queue;
    bool threads_exit;

    // packets number semaphore
    boost::interprocess::interprocess_semaphore packets_queue_sem;
    // cpu threads finished semaphore
    boost::interprocess::interprocess_semaphore threads_finished_sem;
    // mutex for accessing packets_queue
    boost::interprocess::interprocess_mutex packets_queue_mutex;
    // mutex for accessing analyzing_result array
    boost::interprocess::interprocess_mutex analyzing_result_mutex;

    // GPU processing

    bool cuda_init_ok;
    WINDOW_TYPE window_type;
    int buffer_threshold;
    int buffer_flush_time;

    // if equals to 0
    // ac_rules1, packet_buffer1 and tasks1 are used to store packets for processing
    // else ac_rules2, packet_buffer2 and tasks2 are used
    int cur_buffers;
    // pointers to current buffer in double-buffer scheme
    vector<int> *ac_rules;
    vector<u_char> *packet_buffer;
    vector<int> *tasks;

    // pointers to current buffer that should be processed on gpu
    vector<int> *gpu_ac_rules;
    vector<u_char> *gpu_packet_buffer;
    vector<int> *gpu_tasks;

    // double buffered packets, tasks and ac_rules
    vector<int> ac_rules1;
    vector<int> ac_rules2;
    vector<u_char> packet_buffer1;
    vector<u_char> packet_buffer2;
    vector<int> tasks1;
    vector<int> tasks2;

    vector<int> analyze_rules;
    vector<int> matched_rules;

    // CUDA synchronization semaphores
    boost::interprocess::interprocess_semaphore gpu_task_ready;
    boost::interprocess::interprocess_semaphore gpu_task_finished;
    boost::interprocess::interprocess_semaphore gpu_init_finished;
    boost::interprocess::interprocess_mutex gpu_switch_buffer_mutex;

//    // thread which executed tasks on GPU
//    boost::thread gpu_exec_thread;

    // counters
    int num_rules;
    long num_packets;
    long num_tcp_ip_packets;
    long num_bytes;
    long num_tcp_ip_payload_bytes;

    void switch_buffers();

    friend void packet_received(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes);
    friend struct process_on_gpu_callable;
};

#endif // NIDS_H
