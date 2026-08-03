/****************************************************************************
** Meta object code from reading C++ file 'PixelScreenPlugin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../API4/PixelScreenPlugin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PixelScreenPlugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PixelScreenPlugin_t {
    QByteArrayData data[3];
    char stringdata0[31];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PixelScreenPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PixelScreenPlugin_t qt_meta_stringdata_PixelScreenPlugin = {
    {
QT_MOC_LITERAL(0, 0, 17), // "PixelScreenPlugin"
QT_MOC_LITERAL(1, 18, 11), // "RenderFrame"
QT_MOC_LITERAL(2, 30, 0) // ""

    },
    "PixelScreenPlugin\0RenderFrame\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PixelScreenPlugin[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void PixelScreenPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PixelScreenPlugin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RenderFrame(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject PixelScreenPlugin::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_PixelScreenPlugin.data,
    qt_meta_data_PixelScreenPlugin,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PixelScreenPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PixelScreenPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PixelScreenPlugin.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "OpenRGBPluginInterface"))
        return static_cast< OpenRGBPluginInterface*>(this);
    if (!strcmp(_clname, "com.OpenRGBPluginInterface"))
        return static_cast< OpenRGBPluginInterface*>(this);
    return QObject::qt_metacast(_clname);
}

int PixelScreenPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

QT_PLUGIN_METADATA_SECTION
static constexpr unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    0xbf, 
    // "IID"
    0x02,  0x78,  0x1a,  'c',  'o',  'm',  '.',  'O', 
    'p',  'e',  'n',  'R',  'G',  'B',  'P',  'l', 
    'u',  'g',  'i',  'n',  'I',  'n',  't',  'e', 
    'r',  'f',  'a',  'c',  'e', 
    // "className"
    0x03,  0x71,  'P',  'i',  'x',  'e',  'l',  'S', 
    'c',  'r',  'e',  'e',  'n',  'P',  'l',  'u', 
    'g',  'i',  'n', 
    // "MetaData"
    0x04,  0xa9,  0x66,  'C',  'o',  'm',  'm',  'i', 
    't',  0x78,  0x28,  '9',  '6',  'b',  '1',  '2', 
    '9',  '2',  '0',  '1',  '2',  'b',  'a',  '8', 
    'e',  'b',  '1',  'b',  '4',  '2',  '7',  'f', 
    'a',  '5',  '4',  '1',  '4',  '2',  'f',  'f', 
    '1',  '8',  '1',  '2',  '3',  '1',  '6',  '5', 
    '4',  'd',  '9',  0x6b,  'D',  'e',  's',  'c', 
    'r',  'i',  'p',  't',  'i',  'o',  'n',  0x78, 
    0x4f,  'R',  'e',  'n',  'd',  'e',  'r',  ' ', 
    'c',  'u',  's',  't',  'o',  'm',  ' ',  's', 
    'c',  'r',  'o',  'l',  'l',  'i',  'n',  'g', 
    ' ',  't',  'e',  'x',  't',  ',',  ' ',  'c', 
    'l',  'o',  'c',  'k',  ',',  ' ',  's',  'e', 
    'n',  's',  'o',  'r',  's',  ',',  ' ',  'a', 
    'n',  'd',  ' ',  'p',  'i',  'x',  'e',  'l', 
    ' ',  'a',  'r',  't',  ' ',  'o',  'n',  ' ', 
    'O',  'p',  'e',  'n',  'R',  'G',  'B',  ' ', 
    'm',  'a',  't',  'r',  'i',  'c',  'e',  's', 
    0x62,  'I',  'd',  0x78,  0x1d,  'o',  'r',  'g', 
    '.',  'o',  'p',  'e',  'n',  'r',  'g',  'b', 
    '.',  'p',  'i',  'x',  'e',  'l',  's',  'c', 
    'r',  'e',  'e',  'n',  'p',  'l',  'u',  'g', 
    'i',  'n',  0x64,  'N',  'a',  'm',  'e',  0x78, 
    0x1b,  'O',  'p',  'e',  'n',  'R',  'G',  'B', 
    ' ',  'P',  'i',  'x',  'e',  'l',  ' ',  'S', 
    'c',  'r',  'e',  'e',  'n',  ' ',  'P',  'l', 
    'u',  'g',  'i',  'n',  0x77,  'O',  'p',  'e', 
    'n',  'R',  'G',  'B',  'P',  'l',  'u',  'g', 
    'i',  'n',  'A',  'P',  'I',  'V',  'e',  'r', 
    's',  'i',  'o',  'n',  0x04,  0x63,  'U',  'r', 
    'l',  0x78,  0x39,  'h',  't',  't',  'p',  's', 
    ':',  '/',  '/',  'g',  'i',  't',  'h',  'u', 
    'b',  '.',  'c',  'o',  'm',  '/',  'q',  'i', 
    'a',  'n',  'g',  'q',  'i',  'a',  'n',  'g', 
    '1',  '0',  '1',  '/',  'O',  'p',  'e',  'n', 
    'R',  'G',  'B',  'P',  'i',  'x',  'e',  'l', 
    'S',  'c',  'r',  'e',  'e',  'n',  'P',  'l', 
    'u',  'g',  'i',  'n',  0x68,  'V',  'e',  'n', 
    'd',  'o',  'r',  'I',  'd',  0x67,  'o',  'p', 
    'e',  'n',  'r',  'g',  'b',  0x67,  'V',  'e', 
    'r',  's',  'i',  'o',  'n',  0x65,  '0',  '.', 
    '9',  '.',  '6',  0x6a,  'V',  'e',  'r',  's', 
    'i',  'o',  'n',  'S',  't',  'r',  0x6b,  '0', 
    '.',  '9',  '+',  ' ',  '(',  'g',  'i',  't', 
    '6',  ')', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN(PixelScreenPlugin, PixelScreenPlugin)

QT_WARNING_POP
QT_END_MOC_NAMESPACE
