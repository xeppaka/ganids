/********************************************************************************
** Form generated from reading UI file 'ganidsmainwindow.ui'
**
** Created: Fri Apr 6 00:28:12 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GANIDSMAINWINDOW_H
#define UI_GANIDSMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GanidsMainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *button_start;
    QLabel *label;
    QComboBox *combo_interfaces;
    QLabel *label_2;
    QComboBox *combo_cuda_devices;
    QPushButton *button_stop;
    QGroupBox *groupBox;
    QLabel *label_3;
    QLabel *stat_packets_received;
    QLabel *label_4;
    QLabel *stat_tcpip;
    QLabel *label_6;
    QLabel *stat_avg_load;
    QLabel *label_5;
    QLabel *label_7;
    QGroupBox *groupBox_2;
    QCheckBox *check_gpu;
    QGroupBox *group_cpu_settings;
    QSpinBox *spinBox_3;
    QLabel *label_10;
    QGroupBox *group_gpu_settings;
    QLabel *label_8;
    QSpinBox *spin_time;
    QRadioButton *radio_timed;
    QRadioButton *radio_buffer;
    QLabel *label_9;
    QSpinBox *spin_buffer_size;
    QCheckBox *check_save;
    QLineEdit *line_save_path;
    QLineEdit *line_load_path;
    QCheckBox *check_load;
    QPushButton *button_save_path;
    QPushButton *button_load_path;
    QGroupBox *groupBox_3;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GanidsMainWindow)
    {
        if (GanidsMainWindow->objectName().isEmpty())
            GanidsMainWindow->setObjectName(QString::fromUtf8("GanidsMainWindow"));
        GanidsMainWindow->resize(1049, 698);
        centralWidget = new QWidget(GanidsMainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        button_start = new QPushButton(centralWidget);
        button_start->setObjectName(QString::fromUtf8("button_start"));
        button_start->setGeometry(QRect(10, 180, 121, 41));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 15, 171, 18));
        combo_interfaces = new QComboBox(centralWidget);
        combo_interfaces->setObjectName(QString::fromUtf8("combo_interfaces"));
        combo_interfaces->setGeometry(QRect(180, 5, 251, 28));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 61, 141, 18));
        combo_cuda_devices = new QComboBox(centralWidget);
        combo_cuda_devices->setObjectName(QString::fromUtf8("combo_cuda_devices"));
        combo_cuda_devices->setEnabled(false);
        combo_cuda_devices->setGeometry(QRect(180, 54, 251, 28));
        button_stop = new QPushButton(centralWidget);
        button_stop->setObjectName(QString::fromUtf8("button_stop"));
        button_stop->setGeometry(QRect(180, 180, 121, 41));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 450, 431, 151));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 30, 121, 18));
        stat_packets_received = new QLabel(groupBox);
        stat_packets_received->setObjectName(QString::fromUtf8("stat_packets_received"));
        stat_packets_received->setGeometry(QRect(210, 30, 121, 18));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 60, 121, 18));
        stat_tcpip = new QLabel(groupBox);
        stat_tcpip->setObjectName(QString::fromUtf8("stat_tcpip"));
        stat_tcpip->setGeometry(QRect(210, 60, 121, 18));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 90, 131, 18));
        stat_avg_load = new QLabel(groupBox);
        stat_avg_load->setObjectName(QString::fromUtf8("stat_avg_load"));
        stat_avg_load->setGeometry(QRect(210, 90, 121, 18));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 120, 121, 18));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(210, 120, 121, 18));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 230, 481, 211));
        check_gpu = new QCheckBox(centralWidget);
        check_gpu->setObjectName(QString::fromUtf8("check_gpu"));
        check_gpu->setEnabled(true);
        check_gpu->setGeometry(QRect(10, 39, 111, 21));
        group_cpu_settings = new QGroupBox(centralWidget);
        group_cpu_settings->setObjectName(QString::fromUtf8("group_cpu_settings"));
        group_cpu_settings->setGeometry(QRect(450, 10, 251, 71));
        group_cpu_settings->setFlat(true);
        spinBox_3 = new QSpinBox(group_cpu_settings);
        spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));
        spinBox_3->setGeometry(QRect(163, 29, 61, 28));
        label_10 = new QLabel(group_cpu_settings);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(23, 35, 141, 18));
        group_gpu_settings = new QGroupBox(centralWidget);
        group_gpu_settings->setObjectName(QString::fromUtf8("group_gpu_settings"));
        group_gpu_settings->setEnabled(false);
        group_gpu_settings->setGeometry(QRect(730, 10, 281, 101));
        label_8 = new QLabel(group_gpu_settings);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(237, 36, 31, 18));
        spin_time = new QSpinBox(group_gpu_settings);
        spin_time->setObjectName(QString::fromUtf8("spin_time"));
        spin_time->setGeometry(QRect(117, 29, 111, 28));
        spin_time->setMinimum(10);
        spin_time->setMaximum(1000000);
        spin_time->setValue(1000);
        radio_timed = new QRadioButton(group_gpu_settings);
        radio_timed->setObjectName(QString::fromUtf8("radio_timed"));
        radio_timed->setGeometry(QRect(27, 35, 71, 23));
        radio_timed->setChecked(true);
        radio_buffer = new QRadioButton(group_gpu_settings);
        radio_buffer->setObjectName(QString::fromUtf8("radio_buffer"));
        radio_buffer->setGeometry(QRect(27, 66, 71, 23));
        label_9 = new QLabel(group_gpu_settings);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(237, 66, 41, 18));
        spin_buffer_size = new QSpinBox(group_gpu_settings);
        spin_buffer_size->setObjectName(QString::fromUtf8("spin_buffer_size"));
        spin_buffer_size->setGeometry(QRect(117, 59, 111, 28));
        spin_buffer_size->setMinimum(1024);
        spin_buffer_size->setMaximum(100000000);
        spin_buffer_size->setSingleStep(1024);
        spin_buffer_size->setValue(65536);
        check_save = new QCheckBox(centralWidget);
        check_save->setObjectName(QString::fromUtf8("check_save"));
        check_save->setGeometry(QRect(12, 110, 161, 23));
        line_save_path = new QLineEdit(centralWidget);
        line_save_path->setObjectName(QString::fromUtf8("line_save_path"));
        line_save_path->setEnabled(false);
        line_save_path->setGeometry(QRect(180, 102, 491, 28));
        line_load_path = new QLineEdit(centralWidget);
        line_load_path->setObjectName(QString::fromUtf8("line_load_path"));
        line_load_path->setEnabled(false);
        line_load_path->setGeometry(QRect(200, 137, 471, 28));
        check_load = new QCheckBox(centralWidget);
        check_load->setObjectName(QString::fromUtf8("check_load"));
        check_load->setGeometry(QRect(12, 145, 181, 23));
        button_save_path = new QPushButton(centralWidget);
        button_save_path->setObjectName(QString::fromUtf8("button_save_path"));
        button_save_path->setEnabled(false);
        button_save_path->setGeometry(QRect(678, 101, 31, 28));
        button_load_path = new QPushButton(centralWidget);
        button_load_path->setObjectName(QString::fromUtf8("button_load_path"));
        button_load_path->setEnabled(false);
        button_load_path->setGeometry(QRect(678, 137, 31, 28));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(520, 230, 511, 211));
        GanidsMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GanidsMainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1049, 26));
        GanidsMainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(GanidsMainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        GanidsMainWindow->setStatusBar(statusBar);

        retranslateUi(GanidsMainWindow);

        QMetaObject::connectSlotsByName(GanidsMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *GanidsMainWindow)
    {
        GanidsMainWindow->setWindowTitle(QApplication::translate("GanidsMainWindow", "GanidsMainWindow", 0, QApplication::UnicodeUTF8));
        button_start->setText(QApplication::translate("GanidsMainWindow", "Start capture", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GanidsMainWindow", "Select network interface:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("GanidsMainWindow", "Select CUDA device:", 0, QApplication::UnicodeUTF8));
        button_stop->setText(QApplication::translate("GanidsMainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("GanidsMainWindow", "Statistics", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("GanidsMainWindow", "Packets recevied:", 0, QApplication::UnicodeUTF8));
        stat_packets_received->setText(QString());
        label_4->setText(QApplication::translate("GanidsMainWindow", "TCP/IP packets:", 0, QApplication::UnicodeUTF8));
        stat_tcpip->setText(QString());
        label_6->setText(QApplication::translate("GanidsMainWindow", "Avg. load (Mb/s):", 0, QApplication::UnicodeUTF8));
        stat_avg_load->setText(QString());
        label_5->setText(QApplication::translate("GanidsMainWindow", "Cur. load (Mb/s):", 0, QApplication::UnicodeUTF8));
        label_7->setText(QString());
        groupBox_2->setTitle(QApplication::translate("GanidsMainWindow", "Traffic", 0, QApplication::UnicodeUTF8));
        check_gpu->setText(QApplication::translate("GanidsMainWindow", "Enable GPU", 0, QApplication::UnicodeUTF8));
        group_cpu_settings->setTitle(QApplication::translate("GanidsMainWindow", "CPU settings", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("GanidsMainWindow", "Number of threads:", 0, QApplication::UnicodeUTF8));
        group_gpu_settings->setTitle(QApplication::translate("GanidsMainWindow", "GPU settings (buffer flush mode)", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("GanidsMainWindow", "ms", 0, QApplication::UnicodeUTF8));
        radio_timed->setText(QApplication::translate("GanidsMainWindow", "Timed", 0, QApplication::UnicodeUTF8));
        radio_buffer->setText(QApplication::translate("GanidsMainWindow", "Buffer", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("GanidsMainWindow", "bytes", 0, QApplication::UnicodeUTF8));
        check_save->setText(QApplication::translate("GanidsMainWindow", "Save packets to file:", 0, QApplication::UnicodeUTF8));
        check_load->setText(QApplication::translate("GanidsMainWindow", "Load packets from file:", 0, QApplication::UnicodeUTF8));
        button_save_path->setText(QApplication::translate("GanidsMainWindow", "...", 0, QApplication::UnicodeUTF8));
        button_load_path->setText(QApplication::translate("GanidsMainWindow", "...", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("GanidsMainWindow", "CPU load", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GanidsMainWindow: public Ui_GanidsMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GANIDSMAINWINDOW_H
