/****************************************************************************
** Meta object code from reading C++ file 'ConversationsListModel.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../include/DataHandlingPackages/models/ConversationsListModel.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConversationsListModel.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConversationsListModel_t {
    const uint offsetsAndSize[20];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ConversationsListModel_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ConversationsListModel_t qt_meta_stringdata_ConversationsListModel = {
    {
QT_MOC_LITERAL(0, 22), // "ConversationsListModel"
QT_MOC_LITERAL(23, 20), // "currChatIndexChanged"
QT_MOC_LITERAL(44, 0), // ""
QT_MOC_LITERAL(45, 5), // "index"
QT_MOC_LITERAL(51, 20), // "currChatTitleChanged"
QT_MOC_LITERAL(72, 9), // "new_title"
QT_MOC_LITERAL(82, 17), // "popUpConversation"
QT_MOC_LITERAL(100, 16), // "new_inserted_msg"
QT_MOC_LITERAL(117, 13), // "currChatIndex"
QT_MOC_LITERAL(131, 13) // "currChatTitle"

    },
    "ConversationsListModel\0currChatIndexChanged\0"
    "\0index\0currChatTitleChanged\0new_title\0"
    "popUpConversation\0new_inserted_msg\0"
    "currChatIndex\0currChatTitle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConversationsListModel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       2,   41, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x06,    3 /* Public */,
       4,    1,   35,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    1,   38,    2, 0x08,    7 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::UShort,    3,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QJsonObject,    7,

 // properties: name, type, flags
       8, QMetaType::UShort, 0x00015103, uint(0), 0,
       9, QMetaType::QString, 0x00015103, uint(1), 0,

       0        // eod
};

void ConversationsListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConversationsListModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->currChatIndexChanged((*reinterpret_cast< const quint16(*)>(_a[1]))); break;
        case 1: _t->currChatTitleChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->popUpConversation((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ConversationsListModel::*)(const quint16 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConversationsListModel::currChatIndexChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ConversationsListModel::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConversationsListModel::currChatTitleChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ConversationsListModel *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< quint16*>(_v) = _t->currChatIndex(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->currChatTitle(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ConversationsListModel *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setCurrChatIndex(*reinterpret_cast< quint16*>(_v)); break;
        case 1: _t->setCurrChatTitle(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject ConversationsListModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_meta_stringdata_ConversationsListModel.offsetsAndSize,
    qt_meta_data_ConversationsListModel,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ConversationsListModel_t
, QtPrivate::TypeAndForceComplete<quint16, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<ConversationsListModel, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const quint16 &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>


>,
    nullptr
} };


const QMetaObject *ConversationsListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConversationsListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConversationsListModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int ConversationsListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ConversationsListModel::currChatIndexChanged(const quint16 & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConversationsListModel::currChatTitleChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
