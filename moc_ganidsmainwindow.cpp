/****************************************************************************
** Meta object code from reading C++ file 'ganidsmainwindow.h'
**
** Created: Tue May 1 12:45:26 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ganidsmainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ganidsmainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GanidsMainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      31,   18,   17,   17, 0x05,
      67,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     101,   17,   17,   17, 0x08,
     135,  127,   17,   17, 0x08,
     162,  127,   17,   17, 0x08,
     190,  127,   17,   17, 0x08,
     218,   17,   17,   17, 0x08,
     243,   17,   17,   17, 0x08,
     265,   17,   17,   17, 0x08,
     289,   18,   17,   17, 0x08,
     328,   18,   17,   17, 0x08,
     365,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GanidsMainWindow[] = {
    "GanidsMainWindow\0\0message,rule\0"
    "nids_alert(const char*,const char*)\0"
    "nids_log(const char*,const char*)\0"
    "on_button_start_clicked()\0checked\0"
    "on_check_gpu_toggled(bool)\0"
    "on_check_save_toggled(bool)\0"
    "on_check_load_toggled(bool)\0"
    "on_button_stop_clicked()\0on_capture_finished()\0"
    "on_capture_terminated()\0"
    "on_nids_alert(const char*,const char*)\0"
    "on_nids_log(const char*,const char*)\0"
    "on_button_clear_matched_clicked()\0"
};

void GanidsMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GanidsMainWindow *_t = static_cast<GanidsMainWindow *>(_o);
        switch (_id) {
        case 0: _t->nids_alert((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< const char*(*)>(_a[2]))); break;
        case 1: _t->nids_log((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< const char*(*)>(_a[2]))); break;
        case 2: _t->on_button_start_clicked(); break;
        case 3: _t->on_check_gpu_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_check_save_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_check_load_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_button_stop_clicked(); break;
        case 7: _t->on_capture_finished(); break;
        case 8: _t->on_capture_terminated(); break;
        case 9: _t->on_nids_alert((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< const char*(*)>(_a[2]))); break;
        case 10: _t->on_nids_log((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< const char*(*)>(_a[2]))); break;
        case 11: _t->on_button_clear_matched_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GanidsMainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GanidsMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_GanidsMainWindow,
      qt_meta_data_GanidsMainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GanidsMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GanidsMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GanidsMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GanidsMainWindow))
        return static_cast<void*>(const_cast< GanidsMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int GanidsMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void GanidsMainWindow::nids_alert(const char * _t1, const char * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GanidsMainWindow::nids_log(const char * _t1, const char * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
