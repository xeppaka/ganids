#include <QMessageBox>
#include <QComboBox>
#include <QDateTime>
#include "ganidsmainwindow.h"
#include "ui_ganidsmainwindow.h"

void alert_callback(void *user, const char *message, const char *rule) {
    GanidsMainWindow *main_window = reinterpret_cast<GanidsMainWindow *>(user);
    main_window->emit_nids_alert(message, rule);
}

void log_callback(void *user, const char *message, const char *rule) {
    GanidsMainWindow *main_window = reinterpret_cast<GanidsMainWindow *>(user);
    main_window->emit_nids_log(message, rule);
}

GanidsMainWindow::GanidsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GanidsMainWindow),
    gpu_enabled(false),
    nids_capture_thread(NULL),
    timer_id(0)
{
    ui->setupUi(this);
    ui->table_matched_rules->setColumnCount(3);
    QStringList header;
    header << "Time" << "Message" << "Rule";
    ui->table_matched_rules->setHorizontalHeaderLabels(header);
    ui->table_matched_rules->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    connect(this, SIGNAL(nids_alert(const char*,const char*)), this, SLOT(on_nids_alert(const char*,const char*)), Qt::QueuedConnection);
    connect(this, SIGNAL(nids_log(const char*,const char*)), this, SLOT(on_nids_log(const char*,const char*)), Qt::QueuedConnection);

    nids.set_log_level(INFO);
    nids.read_rules("rules.txt");
    nids.add_alert_callback(alert_callback, this);
    nids.add_log_callback(log_callback, this);

    update_network_interfaces();
    update_cuda_devices();

    el_timer.start();
}

GanidsMainWindow::~GanidsMainWindow()
{
    if (nids_capture_thread) {
        stop_capture_thread();

        delete nids_capture_thread;
        nids_capture_thread = NULL;
    }

    if (timer_id)
        killTimer(timer_id);

    delete ui;
}

void GanidsMainWindow::on_button_start_clicked()
{
    QString interface = "wlan0";//ui->combo_interfaces->currentText();
    if (interface.length() <= 0) {
        show_message_interface_not_selected();
        return;
    }

    if (gpu_enabled) {
        int cudaNum = ui->combo_cuda_devices->currentIndex();

        if (cudaNum < 0) {
            show_message_cuda_device_not_selected();
            return;
        }

        WINDOW_TYPE window_type;
        int flush_buffer_size = 0;
        int flush_time = 0;

        if (ui->radio_timed->isChecked()) {
            window_type = TIME;
            flush_time = ui->spin_time->value();
        }
        else {
            window_type = BUF_SIZE;
            flush_buffer_size = ui->spin_buffer_size->value();
        }

        start_gpu_capture_thread(interface, cudaNum, window_type, flush_buffer_size, flush_time);
    } else {
        int num_threads = ui->spin_threads->value();

        start_cpu_capture_thread(interface, num_threads);
    }

    bytes_received = 0;

    lock_ui_for_capture();
    timer_id = startTimer(1000);
    capture_start_time = el_timer.elapsed();
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
        gpu_enabled = false;
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

void GanidsMainWindow::start_gpu_capture_thread(QString interface_name,
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

    connect(nids_capture_thread, SIGNAL(finished()), this, SLOT(on_capture_finished()));
    connect(nids_capture_thread, SIGNAL(terminated()), this, SLOT(on_capture_terminated()));

    if (nids_capture_thread)
        nids_capture_thread->start();
}

void GanidsMainWindow::start_cpu_capture_thread(QString interface,
                                                int threads_num)
{
    nids_capture_thread = new CaptureThread(&nids,
                                            interface,
                                            threads_num);

    connect(nids_capture_thread, SIGNAL(finished()), this, SLOT(on_capture_finished()));
    connect(nids_capture_thread, SIGNAL(terminated()), this, SLOT(on_capture_terminated()));

    if (nids_capture_thread)
        nids_capture_thread->start();
}

void GanidsMainWindow::stop_capture_thread()
{
    nids_capture_thread->stop();
}

void GanidsMainWindow::lock_ui_for_capture()
{
    ui->button_start->setEnabled(false);
    ui->button_stop->setEnabled(true);
    ui->group_cpu_settings->setEnabled(false);
    ui->group_gpu_settings->setEnabled(false);
    ui->combo_cuda_devices->setEnabled(false);
    ui->combo_interfaces->setEnabled(false);
    ui->check_gpu->setEnabled(false);
    ui->check_load->setEnabled(false);
    ui->check_save->setEnabled(false);
}

void GanidsMainWindow::unlock_ui()
{
    ui->button_start->setEnabled(true);
    ui->button_stop->setEnabled(false);
    if (ui->check_gpu->isChecked()) {
        ui->group_gpu_settings->setEnabled(true);
        ui->combo_cuda_devices->setEnabled(true);
    }
    else
        ui->group_cpu_settings->setEnabled(true);
    ui->combo_interfaces->setEnabled(true);
    ui->check_gpu->setEnabled(true);
    ui->check_load->setEnabled(true);
    ui->check_save->setEnabled(true);
}

void GanidsMainWindow::show_message_cuda_device_not_selected()
{
    QMessageBox mb("Information", "CUDA device is not selected. You should select CUDA device from the list.",
                   QMessageBox::Information, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton, this);
    mb.exec();
}

void GanidsMainWindow::show_message_interface_not_selected()
{
    QMessageBox mb("Information", "Network interface is not selected. You should select network interface from the list.",
                   QMessageBox::Information, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton, this);
    mb.exec();
}

void GanidsMainWindow::emit_nids_alert(const char *message, const char *rule)
{
    emit nids_alert(message, rule);
}

void GanidsMainWindow::emit_nids_log(const char *message, const char *rule)
{
    emit nids_log(message, rule);
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
    if (nids_capture_thread)
        stop_capture_thread();

    if (timer_id)
        killTimer(timer_id);

}

void GanidsMainWindow::on_capture_finished()
{
    delete nids_capture_thread;
    nids_capture_thread = NULL;

    unlock_ui();
}

void GanidsMainWindow::on_capture_terminated()
{
    delete nids_capture_thread;
    nids_capture_thread = NULL;

    unlock_ui();
}

void GanidsMainWindow::on_nids_alert(const char *message, const char *rule)
{
    QTableWidgetItem *msg_item = new QTableWidgetItem(QString(message));
    QTableWidgetItem *rule_item = new QTableWidgetItem(QString(rule));
    QTableWidgetItem *time_item = new QTableWidgetItem(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz"));
    time_item->setBackgroundColor(Qt::red);
    int rows = ui->table_matched_rules->rowCount();
    ui->table_matched_rules->setRowCount(rows + 1);
    ui->table_matched_rules->setItem(rows, 0, time_item);
    ui->table_matched_rules->setItem(rows, 1, msg_item);
    ui->table_matched_rules->setItem(rows, 2, rule_item);
}

void GanidsMainWindow::on_nids_log(const char *message, const char *rule)
{
    QTableWidgetItem *msg_item = new QTableWidgetItem(QString(message));
    QTableWidgetItem *rule_item = new QTableWidgetItem(QString(rule));
    QTableWidgetItem *time_item = new QTableWidgetItem(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz"));
    int rows = ui->table_matched_rules->rowCount();
    ui->table_matched_rules->setRowCount(rows + 1);
    ui->table_matched_rules->setItem(rows, 0, time_item);
    ui->table_matched_rules->setItem(rows, 1, msg_item);
    ui->table_matched_rules->setItem(rows, 2, rule_item);
}

void GanidsMainWindow::timerEvent(QTimerEvent *event)
{
    qint64 cur_time = el_timer.elapsed();
    ui->stat_packets_received->setText(QString("%1").arg(nids.num_packets));
    ui->stat_tcpip->setText(QString("%1").arg(nids.num_tcp_ip_packets));
    ui->stat_avg_load->setText(QString("%1").arg(nids.num_bytes / (cur_time - capture_start_time) / 1000.0));
    ui->stat_cur_load->setText(QString("%1").arg((nids.num_bytes - bytes_received) / 1048576.0));
    nids.update_pcap_stats();
    ui->stat_dropped->setText(QString("%1").arg(nids.get_pcap_stats()->ps_drop));
    bytes_received = nids.num_bytes;
}

void GanidsMainWindow::on_button_clear_matched_clicked()
{
    ui->table_matched_rules->clearContents();
    ui->table_matched_rules->setRowCount(0);
}
