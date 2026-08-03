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

    // 0. Enabled Checkbox
    enabledCheck = new QCheckBox("Enable Matrix Text for this device", settingsGroupBox);
    formLayout->addRow("Status:", enabledCheck);

    // 1. Display Mode
    displayModeCombo = new QComboBox(settingsGroupBox);
    displayModeCombo->addItem("Time / Clock");
    displayModeCombo->addItem("Custom Text");
    displayModeCombo->addItem("Pixel Art");
    formLayout->addRow("Display Mode:", displayModeCombo);

    // 2. Font Size
    fontSizeCombo = new QComboBox(settingsGroupBox);
    fontSizeCombo->addItem("Small");
    fontSizeCombo->addItem("Medium");
    fontSizeCombo->addItem("Large");
    fontSizeCombo->addItem("Chinese");
    formLayout->addRow("Font Size:", fontSizeCombo);

    // 3. Custom Text (Multi-line QTextEdit)
    customTextEdit = new QTextEdit(settingsGroupBox);
    customTextEdit->setAcceptRichText(false);
    customTextEdit->setPlainText("OpenRGB");
    customTextEdit->setMaximumHeight(65);
    formLayout->addRow("Custom Text:", customTextEdit);

    // 4. Time Format (Multi-line QTextEdit)
    timeFormatEdit = new QTextEdit(settingsGroupBox);
    timeFormatEdit->setAcceptRichText(false);
    timeFormatEdit->setPlainText("hh:mm tt");
    timeFormatEdit->setMaximumHeight(65);
    formLayout->addRow("Time Format:", timeFormatEdit);

    // Text Alignment Radio Buttons ("Start", "Center", "End")
    alignStartRadio = new QRadioButton("Start", settingsGroupBox);
    alignCenterRadio = new QRadioButton("Center", settingsGroupBox);
    alignEndRadio = new QRadioButton("End", settingsGroupBox);
    alignStartRadio->setChecked(true);

    QHBoxLayout *alignLayout = new QHBoxLayout();
    alignLayout->addWidget(alignStartRadio);
    alignLayout->addWidget(alignCenterRadio);
    alignLayout->addWidget(alignEndRadio);
    alignLayout->addStretch(1);
    formLayout->addRow("Text Alignment:", alignLayout);

    // Pixel Art 2D Matrix Data (Textarea)
    pixelArtEdit = new QTextEdit(settingsGroupBox);
    pixelArtEdit->setPlainText("[ [1, 0, 0, 1], [0, 1, 1, 0], [0, 1, 1, 0], [1, 0, 0, 1] ]");
    pixelArtEdit->setMaximumHeight(90);
    formLayout->addRow("Pixel Art Matrix (JSON):", pixelArtEdit);

    // 5. Scroll Direction
    scrollDirCombo = new QComboBox(settingsGroupBox);
    scrollDirCombo->addItem("Off");
    scrollDirCombo->addItem("Left");
    scrollDirCombo->addItem("Right");
    scrollDirCombo->addItem("Ping-Pong");
    formLayout->addRow("Scroll Direction:", scrollDirCombo);

    // 6. Scroll Speed Slider + Label
    scrollSpeedSlider = new QSlider(Qt::Horizontal, settingsGroupBox);
    scrollSpeedSlider->setRange(1, 100);
    scrollSpeedSlider->setValue(50);
    scrollSpeedValueLabel = new QLabel("50", settingsGroupBox);
    scrollSpeedValueLabel->setMinimumWidth(35);
    QHBoxLayout *speedLayout = new QHBoxLayout();
    speedLayout->addWidget(scrollSpeedSlider);
    speedLayout->addWidget(scrollSpeedValueLabel);
    formLayout->addRow("Scroll Speed:", speedLayout);

    // 7. Text Color
    textColorButton = new QPushButton("Select Text Color", settingsGroupBox);
    formLayout->addRow("Text Color:", textColorButton);

    // 8. FPS Slider + Label
    fpsSlider = new QSlider(Qt::Horizontal, settingsGroupBox);
    fpsSlider->setRange(1, 60);
    fpsSlider->setValue(20);
    fpsValueLabel = new QLabel("20", settingsGroupBox);
    fpsValueLabel->setMinimumWidth(35);
    QHBoxLayout *fpsLayout = new QHBoxLayout();
    fpsLayout->addWidget(fpsSlider);
    fpsLayout->addWidget(fpsValueLabel);
    formLayout->addRow("Frame Rate (FPS):", fpsLayout);

    // 9. Invert Color Checkbox
    invertColorCheck = new QCheckBox("Invert Background / Text Colors", settingsGroupBox);
    formLayout->addRow("Color FX:", invertColorCheck);

    // 10. Padding Offsets
    paddingXSpin = new QSpinBox(settingsGroupBox);
    paddingXSpin->setRange(-100, 100);
    paddingYSpin = new QSpinBox(settingsGroupBox);
    paddingYSpin->setRange(-100, 100);

    QHBoxLayout *padLayout = new QHBoxLayout();
    padLayout->addWidget(new QLabel("X:"));
    padLayout->addWidget(paddingXSpin);
    padLayout->addWidget(new QLabel("Y:"));
    padLayout->addWidget(paddingYSpin);
    padLayout->addStretch(1);
    formLayout->addRow("Position Padding:", padLayout);

    mainLayout->addWidget(settingsGroupBox);
    mainLayout->addStretch(1);

    // Signal connections
    connect(enabledCheck, &QCheckBox::stateChanged, this, &DeviceSettingsPage::on_enabledCheck_stateChanged);
    connect(displayModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DeviceSettingsPage::on_displayModeCombo_currentIndexChanged);
    connect(fontSizeCombo, &QComboBox::currentTextChanged, this, &DeviceSettingsPage::on_fontSizeCombo_currentTextChanged);
    connect(customTextEdit, &QTextEdit::textChanged, this, &DeviceSettingsPage::on_customTextEdit_textChanged);
    connect(timeFormatEdit, &QTextEdit::textChanged, this, &DeviceSettingsPage::on_timeFormatEdit_textChanged);
    connect(alignStartRadio, &QRadioButton::toggled, this, &DeviceSettingsPage::on_alignRadio_toggled);
    connect(alignCenterRadio, &QRadioButton::toggled, this, &DeviceSettingsPage::on_alignRadio_toggled);
    connect(alignEndRadio, &QRadioButton::toggled, this, &DeviceSettingsPage::on_alignRadio_toggled);
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

    int mode_idx = dev_s.display_mode;
    if (mode_idx > 1) mode_idx = (mode_idx == 3) ? 2 : 1;
    displayModeCombo->setCurrentIndex(mode_idx);

    fontSizeCombo->setEnabled(mode_idx != 2);
    timeFormatEdit->setEnabled(mode_idx == 0);
    customTextEdit->setEnabled(mode_idx == 1);
    pixelArtEdit->setEnabled(mode_idx == 2);

    fontSizeCombo->setCurrentText(QString::fromStdString(dev_s.font_size));
    customTextEdit->setPlainText(QString::fromStdString(dev_s.custom_text));
    timeFormatEdit->setPlainText(QString::fromStdString(dev_s.time_format));
    pixelArtEdit->setPlainText(QString::fromStdString(dev_s.pixel_art_json));

    if (dev_s.text_align == 0) alignStartRadio->setChecked(true);
    else if (dev_s.text_align == 1) alignCenterRadio->setChecked(true);
    else if (dev_s.text_align == 2) alignEndRadio->setChecked(true);

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
    dev_s.display_mode = (index == 2) ? 3 : index;
    fontSizeCombo->setEnabled(index != 2);
    timeFormatEdit->setEnabled(index == 0);
    customTextEdit->setEnabled(index == 1);
    pixelArtEdit->setEnabled(index == 2);
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_fontSizeCombo_currentTextChanged(const QString &text)
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.font_size = text.toStdString();
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_customTextEdit_textChanged()
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.custom_text = customTextEdit->toPlainText().toStdString();
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_timeFormatEdit_textChanged()
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.time_format = timeFormatEdit->toPlainText().toStdString();
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_alignRadio_toggled()
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    if (alignStartRadio->isChecked()) dev_s.text_align = 0;
    else if (alignCenterRadio->isChecked()) dev_s.text_align = 1;
    else if (alignEndRadio->isChecked()) dev_s.text_align = 2;
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
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.scroll_speed = value;
    scrollSpeedValueLabel->setText(QString::number(value));
    plugin->SaveSettings();
}

void DeviceSettingsPage::on_textColorButton_clicked()
{
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    QColor current(dev_s.text_r, dev_s.text_g, dev_s.text_b);
    QColor selected = QColorDialog::getColor(current, this, "Select Text Color");
    if (selected.isValid())
    {
        dev_s.text_r = selected.red();
        dev_s.text_g = selected.green();
        dev_s.text_b = selected.blue();
        UpdateColorButton(textColorButton, dev_s.text_r, dev_s.text_g, dev_s.text_b);
        plugin->SaveSettings();
    }
}

void DeviceSettingsPage::on_fpsSlider_valueChanged(int value)
{
    if (loading_ui) return;
    DeviceMatrixSettings& dev_s = plugin->settings.GetForDevice(device_name);
    dev_s.fps = value;
    fpsValueLabel->setText(QString::number(value));
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
