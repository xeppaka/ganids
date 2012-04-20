#include "capturethread.h"

CaptureThread::CaptureThread(Nids *nids,
                             const char *interface_name,
                             int cuda_device_num,
                             WINDOW_TYPE window_type,
                             int flush_buffer_size,
                             int flush_time):
    save_to_file(NULL),
    load_from_file(NULL),
    gpu_enabled(true)
{
    this->nids = nids;
    this->interface_name = interface_name;
    this->cuda_device_num = cuda_device_num;
    this->window_type = window_type;
    this->flush_buffer_size = flush_buffer_size;
    this->flush_time = flush_time;
}

CaptureThread::CaptureThread(Nids *nids,
                             const char *interface_name,
                             int threads_num):
    save_to_file(NULL),
    load_from_file(NULL),
    gpu_enabled(false)
{
    this->nids = nids;
    this->interface_name = interface_name;
    this->threads_num = threads_num;
}

void CaptureThread::run()
{
    if (save_to_file) {
        return;
    }

    if (load_from_file) {
        return;
    }

    qDebug("starting capture thread");

    if (gpu_enabled) {
        nids->start_monitor_gpu(interface_name,
                                cuda_device_num,
                                window_type,
                                flush_buffer_size,
                                flush_time,
                                "nids_log.db");
    } else {
        nids->start_monitor(interface_name,
                            threads_num,
                            "nids_log.db");
    }

    qDebug("Capture thread finished");
}

void CaptureThread::stop()
{
    nids->stop_monitor();
}
