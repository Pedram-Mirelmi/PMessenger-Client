/****************************************************************************
** Meta object code from reading C++ file 'NetworkHandler.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../include/NetworkPackages/NetworkHandler.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetworkHandler.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkHandler_t {
    const uint offsetsAndSize[38];
    char stringdata0[269];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_NetworkHandler_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_NetworkHandler_t qt_meta_stringdata_NetworkHandler = {
    {
QT_MOC_LITERAL(0, 14), // "NetworkHandler"
QT_MOC_LITERAL(15, 19), // "netConnectedChanged"
QT_MOC_LITERAL(35, 0), // ""
QT_MOC_LITERAL(36, 17), // "connection_status"
QT_MOC_LITERAL(54, 15), // "entrySuccessful"
QT_MOC_LITERAL(70, 3), // "msg"
QT_MOC_LITERAL(74, 14), // "newDataArrived"
QT_MOC_LITERAL(89, 22), // "entryNetMessageArrived"
QT_MOC_LITERAL(112, 25), // "newChatCreationMsgArrived"
QT_MOC_LITERAL(138, 15), // "connectToServer"
QT_MOC_LITERAL(154, 19), // "handleNewNetMessage"
QT_MOC_LITERAL(174, 7), // "net_msg"
QT_MOC_LITERAL(182, 15), // "sendRegisterReq"
QT_MOC_LITERAL(198, 8), // "username"
QT_MOC_LITERAL(207, 8), // "password"
QT_MOC_LITERAL(216, 12), // "sendLoginReq"
QT_MOC_LITERAL(229, 21), // "sendUsernameSearchReq"
QT_MOC_LITERAL(251, 4), // "test"
QT_MOC_LITERAL(256, 12) // "netConnected"

    },
    "NetworkHandler\0netConnectedChanged\0\0"
    "connection_status\0entrySuccessful\0msg\0"
    "newDataArrived\0entryNetMessageArrived\0"
    "newChatCreationMsgArrived\0connectToServer\0"
    "handleNewNetMessage\0net_msg\0sendRegisterReq\0"
    "username\0password\0sendLoginReq\0"
    "sendUsernameSearchReq\0test\0netConnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkHandler[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       1,  113, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   80,    2, 0x06,    2 /* Public */,
       4,    1,   83,    2, 0x06,    4 /* Public */,
       6,    1,   86,    2, 0x06,    6 /* Public */,
       7,    1,   89,    2, 0x06,    8 /* Public */,
       8,    1,   92,    2, 0x06,   10 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    0,   95,    2, 0x0a,   12 /* Public */,
      10,    1,   96,    2, 0x0a,   13 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      12,    2,   99,    2, 0x02,   15 /* Public */,
      15,    2,  104,    2, 0x02,   18 /* Public */,
      16,    1,  109,    2, 0x02,   21 /* Public */,
      17,    0,  112,    2, 0x02,   23 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QJsonObject,    5,
    QMetaType::Void, QMetaType::QJsonObject,    5,
    QMetaType::Void, QMetaType::QJsonObject,    5,
    QMetaType::Void, QMetaType::QJsonObject,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,   11,

 // methods: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   14,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,

 // properties: name, type, flags
      18, QMetaType::Bool, 0x00015001, uint(0), 0,

       0        // eod
};

void NetworkHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkHandler *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->netConnectedChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->entrySuccessful((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 2: _t->newDataArrived((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 3: _t->entryNetMessageArrived((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 4: _t->newChatCreationMsgArrived((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 5: _t->connectToServer(); break;
        case 6: _t->handleNewNetMessage((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 7: _t->sendRegisterReq((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: _t->sendLoginReq((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: _t->sendUsernameSearchReq((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->test(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkHandler::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkHandler::netConnectedChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkHandler::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkHandler::entrySuccessful)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkHandler::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkHandler::newDataArrived)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkHandler::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkHandler::entryNetMessageArrived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NetworkHandler::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkHandler::newChatCreationMsgArrived)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<NetworkHandler *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->netConnected(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject NetworkHandler::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NetworkHandler.offsetsAndSize,
    qt_meta_data_NetworkHandler,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_NetworkHandler_t
, QtPrivate::TypeAndForceComplete<bool, std::true_type>, QtPrivate::TypeAndForceComplete<NetworkHandler, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>

>,
    nullptr
} };


const QMetaObject *NetworkHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkHandler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetworkHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void NetworkHandler::netConnectedChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkHandler::entrySuccessful(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetworkHandler::newDataArrived(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkHandler::entryNetMessageArrived(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetworkHandler::newChatCreationMsgArrived(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
