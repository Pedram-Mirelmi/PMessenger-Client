/****************************************************************************
** Meta object code from reading C++ file 'MainApp.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../include/MainApp.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainApp.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainApp_t {
    const uint offsetsAndSize[26];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MainApp_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MainApp_t qt_meta_stringdata_MainApp = {
    {
QT_MOC_LITERAL(0, 7), // "MainApp"
QT_MOC_LITERAL(8, 11), // "initiateApp"
QT_MOC_LITERAL(20, 0), // ""
QT_MOC_LITERAL(21, 25), // "nappendConversationet_msg"
QT_MOC_LITERAL(47, 19), // "openPrivateChatWith"
QT_MOC_LITERAL(67, 7), // "user_id"
QT_MOC_LITERAL(75, 4), // "name"
QT_MOC_LITERAL(80, 19), // "openExistingChatEnv"
QT_MOC_LITERAL(100, 6), // "env_id"
QT_MOC_LITERAL(107, 10), // "is_pending"
QT_MOC_LITERAL(118, 18), // "sendNewTextMessage"
QT_MOC_LITERAL(137, 12), // "message_text"
QT_MOC_LITERAL(150, 10) // "to_pending"

    },
    "MainApp\0initiateApp\0\0nappendConversationet_msg\0"
    "openPrivateChatWith\0user_id\0name\0"
    "openExistingChatEnv\0env_id\0is_pending\0"
    "sendNewTextMessage\0message_text\0"
    "to_pending"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainApp[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x08,    1 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       4,    2,   41,    2, 0x02,    3 /* Public */,
       7,    2,   46,    2, 0x02,    6 /* Public */,
      10,    3,   51,    2, 0x02,    9 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,

 // methods: parameters
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::Bool,    8,    9,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::Bool,    8,   11,   12,

       0        // eod
};

void MainApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainApp *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->initiateApp((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 1: _t->openPrivateChatWith((*reinterpret_cast< const quint64(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->openExistingChatEnv((*reinterpret_cast< const quint64(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 3: _t->sendNewTextMessage((*reinterpret_cast< const quint64(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject MainApp::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MainApp.offsetsAndSize,
    qt_meta_data_MainApp,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MainApp_t
, QtPrivate::TypeAndForceComplete<MainApp, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const quint64, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const quint64 &, std::false_type>, QtPrivate::TypeAndForceComplete<const bool &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const quint64 &, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<const bool &, std::false_type>

>,
    nullptr
} };


const QMetaObject *MainApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainApp::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainApp.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MainApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
