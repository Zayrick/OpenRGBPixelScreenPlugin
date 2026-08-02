/*---------------------------------------------------------*\
| DeviceSettingsPage.cpp                                    |
|                                                           |
|   Individual device tab page for Matrix Text Plugin       |
|                                                           |
|   This file is part of the OpenRGB Matrix Text Plugin     |
|   project                                                 |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "DeviceSettingsPage.h"
#include <QColorDialog>
#include <QColor>

DeviceSettingsPage::DeviceSettingsPage(PixelScreenPlugin* plugin_ptr, const std::string& dev_name, QWidget *parent)
    : QWidget(parent), plugin(plugin_ptr), device_name(dev_name)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QGroupBox *settingsGroupBox = new QGroupBox("Device Text && Animation Settings", this);
    QFormLayout *formLayout = new QFormLayout(settingsGroupBox);

    // Enabled Checkbox
    enabledCheck = new QCheckBox("Enable Matrix Text for this device", settingsGroupBox);
    formLayout->addRow("Status:", enabledCheck);

    // Display Mode
    displayModeCombo = new QComboBox(settingsGroupBox);
    displayModeCombo->addItem("Time / Clock");
    displayModeCombo->addItem("Custom Text");
    displayModeCombo->addItem("Hardware Sensor");
    displayModeCombo->addItem("Pixel Art");
    formLayout->addRow("Display Mode:", displayModeCombo);

    // Font Size
    fontSizeCombo = new QComboBox(settingsGroupBox);
    fontSizeCombo->addItem("Small");
    fontSizeCombo->addItem("Medium");
    fontSizeCombo->addItem("Large");
    fontSizeCombo->addItem("Chinese");
    formLayout->addRow("Font Size:", fontSizeCombo);

    // Custom Text
    customTextEdit = new QLineEdit("OpenRGB", settingsGroupBox);
    formLayout->addRow("Custom Text:", customTextEdit);

    // Time Format
    timeFormatEdit = new QLineEdit("hh:mm tt", settingsGroupBox);
    formLayout->addRow("Time Format:", timeFormatEdit);

    // Hardware Sensor Choice
    sensorCombo = new QComboBox(settingsGroupBox);
    sensorCombo->addItem("CPU Temp");
    sensorCombo->addItem("CPU Load");
    sensorCombo->addItem("GPU Temp");
    sensorCombo->addItem("RAM Usage");
    formLayout->addRow("Hardware Sensor:", sensorCombo);

    // Pixel Art 2D Matrix Data (Textarea)
    pixelArtEdit = new QTextEdit(settingsGroupBox);
    pixelArtEdit->setPlainText("[ [1, 0, 0, 1], [0, 1, 1, 0], [0, 1, 1, 0], [1, 0, 0, 1] ]");
    pixelArtEdit->setMaximumHeight(150);
    formLayout->addRow("Pixel Art Matrix (JSON):", pixelArtEdit);

    // Scroll Direction
    scrollDirCombo = new QComboBox(settingsGroupBox);
    scrollDirCombo->addItem("Off");
    scrollDirCombo->addItem("Left");
    scrollDirCombo->addItem("Right");
    scrollDirCombo->addItem("Ping-Pong");
    formLayout->addRow("Scroll Direction:", scrollDirCombo);

    // Scroll Speed Slider + Label
    scrollSpeedSlider = new QSlider(Qt::Horizontal, settingsGroupBox);
    scrollSpeedSlider->setRange(1, 100);
    scrollSpeedSlider->setValue(50);
    scrollSpeedValueLabel = new QLabel("50", settingsGroupBox);
    scrollSpeedValueLabel->setMinimumWidth(35);
    QHBoxLayout *speedLayout = new QHBoxLayout();
    speedLayout->addWidget(scrollSpeedSlider);
    speedLayout->addWidget(scrollSpeedValueLabel);
    formLayout->addRow("Scroll Speed:", speedLayout);

    // Text Color
    textColorButton = new QPushButton("Select Text Color", settingsGroupBox);
    formLayout->addRow("Text Color:", textColorButton);

    // FPS Slider + Label
    fpsSlider = new QSlider(Qt::Horizontal, settingsGroupBox);
    fpsSlider->setRange(1, 60);
    fpsSlider->setValue(20);
    fpsValueLabel = new QLabel("20", settingsGroupBox);
    fpsValueLabel->setMinimumWidth(35);
    QHBoxLayout *fpsLayout = new QHBoxLayout();
    fpsLayout->addWidget(fpsSlider);
    fpsLayout->addWidget(fpsValueLabel);
    formLayout->addRow("FPS:", fpsLayout);

    // Invert Color
    invertColorCheck = new QCheckBox("Swap background && text color", settingsGroupBox);
    formLayout->addRow("Invert Color:", invertColorCheck);

    // Padding X
    paddingXSpin = new QSpinBox(settingsGroupBox);
    paddingXSpin->setRange(-100, 100);
    paddingXSpin->setValue(0);
    formLayout->addRow("Padding X:", paddingXSpin);

    // Padding Y
    paddingYSpin = new QSpinBox(settingsGroupBox);
    paddingYSpin->setRange(-100, 100);
    paddingYSpin->setValue(0);
    formLayout->addRow("Padding Y:", paddingYSpin);

    mainLayout->addWidget(settingsGroupBox);
    mainLayout->addStretch(1);

    // Signal connections
    connect(enabledCheck, &QCheckBox::stateChanged, this, &DeviceSettingsPage::on_enabledCheck_stateChanged);
    connect(displayModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DeviceSettingsPage::on_displayModeCombo_currentIndexChanged);
    connect(fontSizeCombo, &QComboBox::currentTextChanged, this, &DeviceSettingsPage::on_fontSizeCombo_currentTextChanged);
    connect(customTextEdit, &QLineEdit::textChanged, this, &DeviceSettingsPage::on_customTextEdit_textChanged);
    connect(timeFormatEdit, &QLineEdit::textChanged, this, &DeviceSettingsPage::on_timeFormatEdit_textChanged);
    connect(sensorCombo, &QComboBox::currentTextChanged, this, &DeviceSettingsPage::on_sensorCombo_currentTextChanged);
    connect(pixelArtEdit, &QTextEdit::textChanged, this, &DeviceSettingsPage::on_pixelArtEdit_textChanged);
    connect(scrollDirCombo, &QComboBox::currentTextChanged, this, &DeviceSettingsPage::on_scrollDirCombo_currentTextChanged);
    connect(scrollSpeedSlider, &QSlider::valueChanged, this, &DeviceSettingsPage::on_scrollSpeedSlider_valueChanged);
    connect(textColorButton, &QPushButton::clicked, this, &DeviceSettingsPage::on_textColorButton_clicked);
    connect(fpsSlider, &QSlider::valueChanged, this, &DeviceSettingsPage::on_fpsSlider_valueChanged);
    connect(invertColorCheck, &QCheckBox::stateChanged, this, &DeviceSettingsPage::on_invertColorCheck_stateChanged);
    connect(paddingXSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &DeviceSettingsPage::on_paddingXSpin_valueChanged);
    connect(paddingYSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &DeviceSettingsPage::on_paddingYSpin_valueChanged);

    LoadSettingsToPage();
}

void DeviceSettingsPage::LoadSettingsToPage()
{
    loading_ui = true;

    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);

    enabledCheck->setChecked(dev_s.enabled);

    displayModeCombo->setCurrentIndex(dev_s.display_mode);
    fontSizeCombo->setEnabled(dev_s.display_mode != 3);
    timeFormatEdit->setEnabled(dev_s.display_mode == 0);
    customTextEdit->setEnabled(dev_s.display_mode == 1);
    sensorCombo->setEnabled(dev_s.display_mode == 2);
    pixelArtEdit->setEnabled(dev_s.display_mode == 3);

    fontSizeCombo->setCurrentText(QString::fromStdString(dev_s.font_size));
    customTextEdit->setText(QString::fromStdString(dev_s.custom_text));
    timeFormatEdit->setText(QString::fromStdString(dev_s.time_format));
    sensorCombo->setCurrentText(QString::fromStdString(dev_s.sensor_type));
    pixelArtEdit->setPlainText(QString::fromStdString(dev_s.pixel_art_json));

    scrollDirCombo->setCurrentText(QString::fromStdString(dev_s.scroll_direction));
    scrollSpeedSlider->setValue(dev_s.scroll_speed);
    scrollSpeedValueLabel->setText(QString::number(dev_s.scroll_speed));

    fpsSlider->setValue(dev_s.fps);
    fpsValueLabel->setText(QString::number(dev_s.fps));

    invertColorCheck->setChecked(dev_s.invert_color);
    paddingXSpin->setValue(dev_s.padding_x);
    paddingYSpin->setValue(dev_s.padding_y);

    UpdateColorButton(textColorButton, dev_s.text_r, dev_s.text_g, dev_s.text_b);

    loading_ui = false;
}

void DeviceSettingsPage::UpdateColorButton(QPushButton* button, unsigned char r, unsigned char g, unsigned char b)
{
    QString stylesheet = QString("background-color: rgb(%1, %2, %3); color: %4; font-weight: bold;")
        .arg(r).arg(g).arg(b)
        .arg((r*0.299 + g*0.587 + b*0.114) > 128 ? "black" : "white");
    button->setStyleSheet(stylesheet);
}

void DeviceSettingsPage::on_enabledCheck_stateChanged(int state)
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.enabled = (state == Qt::Checked);
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_displayModeCombo_currentIndexChanged(int index)
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.display_mode = index;
    fontSizeCombo->setEnabled(index != 3);
    timeFormatEdit->setEnabled(index == 0);
    customTextEdit->setEnabled(index == 1);
    sensorCombo->setEnabled(index == 2);
    pixelArtEdit->setEnabled(index == 3);
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_fontSizeCombo_currentTextChanged(const QString &text)
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.font_size = text.toStdString();
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_customTextEdit_textChanged(const QString &text)
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.custom_text = text.toStdString();
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_timeFormatEdit_textChanged(const QString &text)
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.time_format = text.toStdString();
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_sensorCombo_currentTextChanged(const QString &text)
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.sensor_type = text.toStdString();
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_pixelArtEdit_textChanged()
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.pixel_art_json = pixelArtEdit->toPlainText().toStdString();
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_scrollDirCombo_currentTextChanged(const QString &text)
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.scroll_direction = text.toStdString();
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_scrollSpeedSlider_valueChanged(int value)
{
    scrollSpeedValueLabel->setText(QString::number(value));
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.scroll_speed = value;
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_textColorButton_clicked()
{
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    QColor current = QColor(dev_s.text_r, dev_s.text_g, dev_s.text_b);
    QColor color = QColorDialog::getColor(current, this, "Select Text Color");
    if (color.isValid())
    {
        dev_s.text_r = color.red();
        dev_s.text_g = color.green();
        dev_s.text_b = color.blue();
        UpdateColorButton(textColorButton, dev_s.text_r, dev_s.text_g, dev_s.text_b);
        plugin->SaveSettings();
    }
}

void DeviceSettingsPage::on_fpsSlider_valueChanged(int value)
{
    fpsValueLabel->setText(QString::number(value));
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.fps = value;
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_invertColorCheck_stateChanged(int state)
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.invert_color = (state == Qt::Checked);
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_paddingXSpin_valueChanged(int value)
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.padding_x = value;
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_paddingYSpin_valueChanged(int value)
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.padding_y = value;
    plugin->SaveSettings();
}
