#include "ganidsmainwindow.h"
#include "ui_ganidsmainwindow.h"

GanidsMainWindow::GanidsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GanidsMainWindow),
    gpu_enabled(false),
    nids_capture_thread(NULL)
{
    ui->setupUi(this);

    update_network_interfaces();
    update_cuda_devices();
}

GanidsMainWindow::~GanidsMainWindow()
{
    if (nids_capture_thread) {
        stop_capture_thread();

        delete nids_capture_thread;
        nids_capture_thread = NULL;
    }
    delete ui;
}

void GanidsMainWindow::on_button_start_clicked()
{
    if (gpu_enabled) {
        QString interface = ui->combo_interfaces->currentText();
        int cudaNum = ui->combo_cuda_devices->currentIndex();

        if (cudaNum < 0) {
            show_message_cuda_device_not_selected();
            return;
        }

        if (interface.length() <= 0) {
            show_message_interface_not_selected();
            return;
        }

        QByteArray ba = interface.toLocal8Bit();
        WINDOW_TYPE window_type;
        int flush_buffer_size = 0;
        int flush_time = 0;

        if (ui->radio_timed->isEnabled()) {
            window_type = TIME;
            flush_time = ui->spin_time->value();
        }
        else {
            window_type = BUF_SIZE;
            flush_buffer_size = ui->spin_buffer_size->value();
        }

        start_gpu_capture_thread(ba.data(), cudaNum, window_type, flush_buffer_size, flush_time);
    }
}

void GanidsMainWindow::on_check_gpu_toggled(bool checked)
{
    if (checked) {
        update_cuda_devices();
        if (ui->combo_cuda_devices->count() > 0) {
            gpu_enabled = true;
            ui->combo_cuda_devices->setEnabled(true);
            ui->group_gpu_settings->setEnabled(true);
            ui->group_cpu_settings->setEnabled(false);
        }
    } else {
        ui->combo_cuda_devices->setEnabled(false);
        ui->group_gpu_settings->setEnabled(false);
        ui->group_cpu_settings->setEnabled(true);
    }
}

void GanidsMainWindow::update_network_interfaces()
{
    ui->combo_interfaces->clear();

    vector<string> interfaces;
    if (nids.list_interfaces(interfaces)) {
        for (vector<string>::iterator it = interfaces.begin(); it != interfaces.end(); ++it)
            ui->combo_interfaces->addItem(QString((*it).c_str()));
    }
}

void GanidsMainWindow::update_cuda_devices()
{
    ui->combo_cuda_devices->clear();

    vector<cudaDeviceProp> cuda_devices;
    if (nids.list_cuda_devices(cuda_devices)) {
        for (vector<cudaDeviceProp>::iterator it = cuda_devices.begin(); it != cuda_devices.end(); ++it)
            ui->combo_cuda_devices->addItem(QString((*it).name));
    }
}

void GanidsMainWindow::start_gpu_capture_thread(const char *interface_name,
                                                int cuda_device_num,
                                                WINDOW_TYPE window_type,
                                                int flush_buffer_size,
                                                int flush_time)
{
    nids_capture_thread = new CaptureThread(&nids,
                                            interface_name,
                                            cuda_device_num,
                                            window_type,
                                            flush_buffer_size,
                                            flush_time);
    if (nids_capture_thread)
        nids_capture_thread->start();
}

void GanidsMainWindow::start_cpu_capture_thread(const char *interface_name,
                                                int threads_num)
{
    nids_capture_thread = new CaptureThread(&nids,
                                            interface_name,
                                            threads_num);

    if (nids_capture_thread)
        nids_capture_thread->start();
}

void GanidsMainWindow::stop_capture_thread()
{
    nids_capture_thread->stop();
    nids_capture_thread->wait(5000);
    if (!nids_capture_thread->isFinished()) {
        nids_capture_thread->terminate();
        nids_capture_thread->wait(5000);
    }
}

void GanidsMainWindow::show_message_cuda_device_not_selected()
{
}

void GanidsMainWindow::show_message_interface_not_selected()
{
}

void GanidsMainWindow::on_check_save_toggled(bool checked)
{
    ui->line_save_path->setEnabled(checked);
    ui->button_save_path->setEnabled(checked);

    if (checked) {
        ui->check_load->setChecked(false);
        ui->line_load_path->setEnabled(false);
        ui->button_load_path->setEnabled(false);
    }
}

void GanidsMainWindow::on_check_load_toggled(bool checked)
{
    ui->line_load_path->setEnabled(checked);
    ui->button_load_path->setEnabled(checked);

    if (checked) {
        ui->check_save->setChecked(!checked);
        ui->line_save_path->setEnabled(!checked);
        ui->button_save_path->setEnabled(!checked);
    }
}

void GanidsMainWindow::on_button_stop_clicked()
{
    if (nids_capture_thread) {
        stop_capture_thread();

        delete nids_capture_thread;
        nids_capture_thread = NULL;
    }
}
