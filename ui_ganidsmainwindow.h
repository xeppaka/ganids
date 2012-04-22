/********************************************************************************
** Form generated from reading UI file 'ganidsmainwindow.ui'
**
** Created: Sun Apr 22 22:55:17 2012
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
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GanidsMainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QPushButton *button_load_path;
    QCheckBox *check_save;
    QGroupBox *group_gpu_settings;
    QLabel *label_8;
    QSpinBox *spin_time;
    QRadioButton *radio_timed;
    QRadioButton *radio_buffer;
    QLabel *label_9;
    QSpinBox *spin_buffer_size;
    QLabel *label;
    QCheckBox *check_load;
    QLineEdit *line_load_path;
    QLabel *label_2;
    QComboBox *combo_interfaces;
    QCheckBox *check_gpu;
    QGroupBox *group_cpu_settings;
    QSpinBox *spin_threads;
    QLabel *label_10;
    QLineEdit *line_save_path;
    QComboBox *combo_cuda_devices;
    QPushButton *button_save_path;
    QPushButton *button_stop;
    QPushButton *button_start;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_3;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *stat_avg_load;
    QLabel *label_3;
    QLabel *stat_packets_received;
    QLabel *stat_tcpip;
    QLabel *stat_cur_load;
    QPushButton *button_clear_matched;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_3;
    QTableWidget *table_matched_rules;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GanidsMainWindow)
    {
        if (GanidsMainWindow->objectName().isEmpty())
            GanidsMainWindow->setObjectName(QString::fromUtf8("GanidsMainWindow"));
        GanidsMainWindow->resize(1030, 657);
        centralWidget = new QWidget(GanidsMainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(0, 215));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        button_load_path = new QPushButton(frame);
        button_load_path->setObjectName(QString::fromUtf8("button_load_path"));
        button_load_path->setEnabled(false);
        button_load_path->setGeometry(QRect(678, 135, 31, 28));
        check_save = new QCheckBox(frame);
        check_save->setObjectName(QString::fromUtf8("check_save"));
        check_save->setGeometry(QRect(12, 108, 161, 23));
        group_gpu_settings = new QGroupBox(frame);
        group_gpu_settings->setObjectName(QString::fromUtf8("group_gpu_settings"));
        group_gpu_settings->setEnabled(false);
        group_gpu_settings->setGeometry(QRect(730, 8, 281, 101));
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
        spin_buffer_size->setMinimum(32);
        spin_buffer_size->setMaximum(100000000);
        spin_buffer_size->setSingleStep(1024);
        spin_buffer_size->setValue(65536);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 13, 171, 18));
        check_load = new QCheckBox(frame);
        check_load->setObjectName(QString::fromUtf8("check_load"));
        check_load->setGeometry(QRect(12, 143, 181, 23));
        line_load_path = new QLineEdit(frame);
        line_load_path->setObjectName(QString::fromUtf8("line_load_path"));
        line_load_path->setEnabled(false);
        line_load_path->setGeometry(QRect(200, 135, 471, 28));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 59, 141, 18));
        combo_interfaces = new QComboBox(frame);
        combo_interfaces->setObjectName(QString::fromUtf8("combo_interfaces"));
        combo_interfaces->setGeometry(QRect(180, 3, 251, 28));
        check_gpu = new QCheckBox(frame);
        check_gpu->setObjectName(QString::fromUtf8("check_gpu"));
        check_gpu->setEnabled(true);
        check_gpu->setGeometry(QRect(10, 37, 111, 21));
        group_cpu_settings = new QGroupBox(frame);
        group_cpu_settings->setObjectName(QString::fromUtf8("group_cpu_settings"));
        group_cpu_settings->setGeometry(QRect(450, 8, 251, 71));
        group_cpu_settings->setFlat(true);
        spin_threads = new QSpinBox(group_cpu_settings);
        spin_threads->setObjectName(QString::fromUtf8("spin_threads"));
        spin_threads->setGeometry(QRect(163, 29, 61, 28));
        spin_threads->setMinimum(1);
        spin_threads->setMaximum(9999);
        spin_threads->setValue(1);
        label_10 = new QLabel(group_cpu_settings);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(23, 35, 141, 18));
        line_save_path = new QLineEdit(frame);
        line_save_path->setObjectName(QString::fromUtf8("line_save_path"));
        line_save_path->setEnabled(false);
        line_save_path->setGeometry(QRect(180, 100, 491, 28));
        combo_cuda_devices = new QComboBox(frame);
        combo_cuda_devices->setObjectName(QString::fromUtf8("combo_cuda_devices"));
        combo_cuda_devices->setEnabled(false);
        combo_cuda_devices->setGeometry(QRect(180, 52, 251, 28));
        button_save_path = new QPushButton(frame);
        button_save_path->setObjectName(QString::fromUtf8("button_save_path"));
        button_save_path->setEnabled(false);
        button_save_path->setGeometry(QRect(678, 99, 31, 28));
        button_stop = new QPushButton(frame);
        button_stop->setObjectName(QString::fromUtf8("button_stop"));
        button_stop->setEnabled(false);
        button_stop->setGeometry(QRect(170, 170, 121, 41));
        button_start = new QPushButton(frame);
        button_start->setObjectName(QString::fromUtf8("button_start"));
        button_start->setGeometry(QRect(10, 170, 121, 41));

        verticalLayout->addWidget(frame);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));

        horizontalLayout->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));

        horizontalLayout->addWidget(groupBox_2);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(250, 0));
        formLayout = new QFormLayout(groupBox);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_5);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(6, QFormLayout::FieldRole, label_7);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_6);

        stat_avg_load = new QLabel(groupBox);
        stat_avg_load->setObjectName(QString::fromUtf8("stat_avg_load"));

        formLayout->setWidget(1, QFormLayout::FieldRole, stat_avg_load);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        stat_packets_received = new QLabel(groupBox);
        stat_packets_received->setObjectName(QString::fromUtf8("stat_packets_received"));

        formLayout->setWidget(2, QFormLayout::FieldRole, stat_packets_received);

        stat_tcpip = new QLabel(groupBox);
        stat_tcpip->setObjectName(QString::fromUtf8("stat_tcpip"));

        formLayout->setWidget(3, QFormLayout::FieldRole, stat_tcpip);

        stat_cur_load = new QLabel(groupBox);
        stat_cur_load->setObjectName(QString::fromUtf8("stat_cur_load"));

        formLayout->setWidget(0, QFormLayout::FieldRole, stat_cur_load);

        button_clear_matched = new QPushButton(groupBox);
        button_clear_matched->setObjectName(QString::fromUtf8("button_clear_matched"));

        formLayout->setWidget(4, QFormLayout::LabelRole, button_clear_matched);


        horizontalLayout_2->addWidget(groupBox);

        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_4);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        table_matched_rules = new QTableWidget(groupBox_4);
        table_matched_rules->setObjectName(QString::fromUtf8("table_matched_rules"));

        horizontalLayout_3->addWidget(table_matched_rules);


        horizontalLayout_2->addWidget(groupBox_4);


        verticalLayout->addLayout(horizontalLayout_2);

        GanidsMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GanidsMainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1030, 26));
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
        button_load_path->setText(QApplication::translate("GanidsMainWindow", "...", 0, QApplication::UnicodeUTF8));
        check_save->setText(QApplication::translate("GanidsMainWindow", "Save packets to file:", 0, QApplication::UnicodeUTF8));
        group_gpu_settings->setTitle(QApplication::translate("GanidsMainWindow", "GPU settings (buffer flush mode)", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("GanidsMainWindow", "ms", 0, QApplication::UnicodeUTF8));
        radio_timed->setText(QApplication::translate("GanidsMainWindow", "Timed", 0, QApplication::UnicodeUTF8));
        radio_buffer->setText(QApplication::translate("GanidsMainWindow", "Buffer", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("GanidsMainWindow", "bytes", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GanidsMainWindow", "Select network interface:", 0, QApplication::UnicodeUTF8));
        check_load->setText(QApplication::translate("GanidsMainWindow", "Load packets from file:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("GanidsMainWindow", "Select CUDA device:", 0, QApplication::UnicodeUTF8));
        check_gpu->setText(QApplication::translate("GanidsMainWindow", "Enable GPU", 0, QApplication::UnicodeUTF8));
        group_cpu_settings->setTitle(QApplication::translate("GanidsMainWindow", "CPU settings", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("GanidsMainWindow", "Number of threads:", 0, QApplication::UnicodeUTF8));
        button_save_path->setText(QApplication::translate("GanidsMainWindow", "...", 0, QApplication::UnicodeUTF8));
        button_stop->setText(QApplication::translate("GanidsMainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        button_start->setText(QApplication::translate("GanidsMainWindow", "Start capture", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("GanidsMainWindow", "CPU load", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("GanidsMainWindow", "Traffic", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("GanidsMainWindow", "Statistics", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("GanidsMainWindow", "Cur. load (Mb/s):", 0, QApplication::UnicodeUTF8));
        label_7->setText(QString());
        label_4->setText(QApplication::translate("GanidsMainWindow", "TCP/IP packets:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("GanidsMainWindow", "Avg. load (Mb/s):", 0, QApplication::UnicodeUTF8));
        stat_avg_load->setText(QString());
        label_3->setText(QApplication::translate("GanidsMainWindow", "Packets recevied:", 0, QApplication::UnicodeUTF8));
        stat_packets_received->setText(QString());
        stat_tcpip->setText(QString());
        stat_cur_load->setText(QString());
        button_clear_matched->setText(QApplication::translate("GanidsMainWindow", "Clear table", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("GanidsMainWindow", "Matched rules", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GanidsMainWindow: public Ui_GanidsMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GANIDSMAINWINDOW_H
