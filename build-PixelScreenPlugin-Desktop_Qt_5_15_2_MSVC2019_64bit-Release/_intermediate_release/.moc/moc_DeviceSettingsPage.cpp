/****************************************************************************
** Meta object code from reading C++ file 'DeviceSettingsPage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../API4/DeviceSettingsPage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DeviceSettingsPage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DeviceSettingsPage_t {
    QByteArrayData data[20];
    char stringdata0[473];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DeviceSettingsPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DeviceSettingsPage_t qt_meta_stringdata_DeviceSettingsPage = {
    {
QT_MOC_LITERAL(0, 0, 18), // "DeviceSettingsPage"
QT_MOC_LITERAL(1, 19, 28), // "on_enabledCheck_stateChanged"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 5), // "state"
QT_MOC_LITERAL(4, 55, 39), // "on_displayModeCombo_currentIn..."
QT_MOC_LITERAL(5, 95, 5), // "index"
QT_MOC_LITERAL(6, 101, 35), // "on_fontSizeCombo_currentTextC..."
QT_MOC_LITERAL(7, 137, 4), // "text"
QT_MOC_LITERAL(8, 142, 29), // "on_customTextEdit_textChanged"
QT_MOC_LITERAL(9, 172, 29), // "on_timeFormatEdit_textChanged"
QT_MOC_LITERAL(10, 202, 21), // "on_alignRadio_toggled"
QT_MOC_LITERAL(11, 224, 27), // "on_pixelArtEdit_textChanged"
QT_MOC_LITERAL(12, 252, 36), // "on_scrollDirCombo_currentText..."
QT_MOC_LITERAL(13, 289, 33), // "on_scrollSpeedSlider_valueCha..."
QT_MOC_LITERAL(14, 323, 5), // "value"
QT_MOC_LITERAL(15, 329, 26), // "on_textColorButton_clicked"
QT_MOC_LITERAL(16, 356, 25), // "on_fpsSlider_valueChanged"
QT_MOC_LITERAL(17, 382, 32), // "on_invertColorCheck_stateChanged"
QT_MOC_LITERAL(18, 415, 28), // "on_paddingXSpin_valueChanged"
QT_MOC_LITERAL(19, 444, 28) // "on_paddingYSpin_valueChanged"

    },
    "DeviceSettingsPage\0on_enabledCheck_stateChanged\0"
    "\0state\0on_displayModeCombo_currentIndexChanged\0"
    "index\0on_fontSizeCombo_currentTextChanged\0"
    "text\0on_customTextEdit_textChanged\0"
    "on_timeFormatEdit_textChanged\0"
    "on_alignRadio_toggled\0on_pixelArtEdit_textChanged\0"
    "on_scrollDirCombo_currentTextChanged\0"
    "on_scrollSpeedSlider_valueChanged\0"
    "value\0on_textColorButton_clicked\0"
    "on_fpsSlider_valueChanged\0"
    "on_invertColorCheck_stateChanged\0"
    "on_paddingXSpin_valueChanged\0"
    "on_paddingYSpin_valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DeviceSettingsPage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x08 /* Private */,
       4,    1,   87,    2, 0x08 /* Private */,
       6,    1,   90,    2, 0x08 /* Private */,
       8,    0,   93,    2, 0x08 /* Private */,
       9,    0,   94,    2, 0x08 /* Private */,
      10,    0,   95,    2, 0x08 /* Private */,
      11,    0,   96,    2, 0x08 /* Private */,
      12,    1,   97,    2, 0x08 /* Private */,
      13,    1,  100,    2, 0x08 /* Private */,
      15,    0,  103,    2, 0x08 /* Private */,
      16,    1,  104,    2, 0x08 /* Private */,
      17,    1,  107,    2, 0x08 /* Private */,
      18,    1,  110,    2, 0x08 /* Private */,
      19,    1,  113,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,

       0        // eod
};

void DeviceSettingsPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DeviceSettingsPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_enabledCheck_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_displayModeCombo_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_fontSizeCombo_currentTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->on_customTextEdit_textChanged(); break;
        case 4: _t->on_timeFormatEdit_textChanged(); break;
        case 5: _t->on_alignRadio_toggled(); break;
        case 6: _t->on_pixelArtEdit_textChanged(); break;
        case 7: _t->on_scrollDirCombo_currentTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->on_scrollSpeedSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_textColorButton_clicked(); break;
        case 10: _t->on_fpsSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_invertColorCheck_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->on_paddingXSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_paddingYSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DeviceSettingsPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_DeviceSettingsPage.data,
    qt_meta_data_DeviceSettingsPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DeviceSettingsPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DeviceSettingsPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DeviceSettingsPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DeviceSettingsPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
