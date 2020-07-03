/****************************************************************************
** Meta object code from reading C++ file 'commandfunop.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../PLCDriveCode/commandOp/commandfunop.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'commandfunop.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CommandFunOp_t {
    QByteArrayData data[15];
    char stringdata0[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommandFunOp_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommandFunOp_t qt_meta_stringdata_CommandFunOp = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CommandFunOp"
QT_MOC_LITERAL(1, 13, 14), // "signalShowInfo"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3), // "msg"
QT_MOC_LITERAL(4, 33, 14), // "signaltaskInfo"
QT_MOC_LITERAL(5, 48, 17), // "slotRecPlcCmdData"
QT_MOC_LITERAL(6, 66, 7), // "uint8_t"
QT_MOC_LITERAL(7, 74, 6), // "funcmd"
QT_MOC_LITERAL(8, 81, 7), // "cmdstru"
QT_MOC_LITERAL(9, 89, 4), // "stru"
QT_MOC_LITERAL(10, 94, 16), // "slotMonitorTimer"
QT_MOC_LITERAL(11, 111, 10), // "slotRunEnd"
QT_MOC_LITERAL(12, 122, 3), // "cmd"
QT_MOC_LITERAL(13, 126, 6), // "taskid"
QT_MOC_LITERAL(14, 133, 24) // "slotMonitorRunStateTimer"

    },
    "CommandFunOp\0signalShowInfo\0\0msg\0"
    "signaltaskInfo\0slotRecPlcCmdData\0"
    "uint8_t\0funcmd\0cmdstru\0stru\0"
    "slotMonitorTimer\0slotRunEnd\0cmd\0taskid\0"
    "slotMonitorRunStateTimer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommandFunOp[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   50,    2, 0x0a /* Public */,
      10,    0,   55,    2, 0x08 /* Private */,
      11,    4,   56,    2, 0x08 /* Private */,
      14,    0,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 8,    7,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 8, QMetaType::Int, QMetaType::QString,   12,    9,   13,    3,
    QMetaType::Void,

       0        // eod
};

void CommandFunOp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CommandFunOp *_t = static_cast<CommandFunOp *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalShowInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->signaltaskInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->slotRecPlcCmdData((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< cmdstru(*)>(_a[2]))); break;
        case 3: _t->slotMonitorTimer(); break;
        case 4: _t->slotRunEnd((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< cmdstru(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 5: _t->slotMonitorRunStateTimer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CommandFunOp::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommandFunOp::signalShowInfo)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CommandFunOp::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommandFunOp::signaltaskInfo)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject CommandFunOp::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CommandFunOp.data,
      qt_meta_data_CommandFunOp,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CommandFunOp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommandFunOp::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CommandFunOp.stringdata0))
        return static_cast<void*>(const_cast< CommandFunOp*>(this));
    return QObject::qt_metacast(_clname);
}

int CommandFunOp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CommandFunOp::signalShowInfo(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CommandFunOp::signaltaskInfo(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
