#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>
#include "nids.h"

class CaptureThread : public QThread
{
public:
    CaptureThread(Nids *nids, QString interface_name, int cuda_device_num, WINDOW_TYPE window_type, int flush_buffer_size, int flush_time);
    CaptureThread(Nids *nids, QString interface_name, int threads_num);
    void run();
    void stop();

private:
    bool gpu_enabled;
    Nids *nids;
    QString interface_name;
    int cuda_device_num;
    WINDOW_TYPE window_type;
    int flush_buffer_size;
    int flush_time;
    int threads_num;
    const char *save_to_file;
    const char *load_from_file;
};

#endif // CAPTURETHREAD_H
