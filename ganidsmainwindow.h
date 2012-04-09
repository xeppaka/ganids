#ifndef GANIDSMAINWINDOW_H
#define GANIDSMAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include "nids.h"
#include "capturethread.h"

namespace Ui {
class GanidsMainWindow;
}

class GanidsMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GanidsMainWindow(QWidget *parent = 0);
    ~GanidsMainWindow();
    
private slots:
    void on_button_start_clicked();

    void on_check_gpu_toggled(bool checked);

    void on_check_save_toggled(bool checked);

    void on_check_load_toggled(bool checked);

    void on_button_stop_clicked();

protected:
    void timerEvent(QTimerEvent *event);

private:
    Ui::GanidsMainWindow *ui;

    void update_network_interfaces();
    void update_cuda_devices();
    void start_gpu_capture_thread(const char *interface_name,
                                  int cuda_device_num,
                                  WINDOW_TYPE window_type,
                                  int flush_buffer_size,
                                  int flush_time);

    void start_cpu_capture_thread(const char *interface_name, int threads_num);
    void stop_capture_thread();

    void lock_ui_for_capture();
    void unlock_ui();

    void show_message_cuda_device_not_selected();
    void show_message_interface_not_selected();

    CaptureThread *nids_capture_thread;
    Nids nids;

    bool gpu_enabled;

    int timer_id;
    QElapsedTimer el_timer;
    qint64 capture_start_time;
    long bytes_received;
};

#endif // GANIDSMAINWINDOW_H
