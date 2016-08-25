/****************************************************************************
** Meta object code from reading C++ file 'sockettest.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Basic/sockettest.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sockettest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SocketTest_t {
    QByteArrayData data[10];
    char stringdata0[86];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SocketTest_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SocketTest_t qt_meta_stringdata_SocketTest = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SocketTest"
QT_MOC_LITERAL(1, 11, 7), // "newData"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 5), // "value"
QT_MOC_LITERAL(4, 26, 7), // "unitSel"
QT_MOC_LITERAL(5, 34, 9), // "connected"
QT_MOC_LITERAL(6, 44, 12), // "disconnected"
QT_MOC_LITERAL(7, 57, 12), // "bytesWritten"
QT_MOC_LITERAL(8, 70, 5), // "bytes"
QT_MOC_LITERAL(9, 76, 9) // "readyRead"

    },
    "SocketTest\0newData\0\0value\0unitSel\0"
    "connected\0disconnected\0bytesWritten\0"
    "bytes\0readyRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SocketTest[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   44,    2, 0x0a /* Public */,
       6,    0,   45,    2, 0x0a /* Public */,
       7,    1,   46,    2, 0x0a /* Public */,
       9,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,    8,
    QMetaType::Void,

       0        // eod
};

void SocketTest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SocketTest *_t = static_cast<SocketTest *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newData((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->connected(); break;
        case 2: _t->disconnected(); break;
        case 3: _t->bytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 4: _t->readyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SocketTest::*_t)(float , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SocketTest::newData)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject SocketTest::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SocketTest.data,
      qt_meta_data_SocketTest,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SocketTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SocketTest::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SocketTest.stringdata0))
        return static_cast<void*>(const_cast< SocketTest*>(this));
    return QObject::qt_metacast(_clname);
}

int SocketTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SocketTest::newData(float _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
