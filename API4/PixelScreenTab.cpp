/*---------------------------------------------------------*\
| OpenRGBMatrixTextTab.cpp                                  |
|                                                           |
|   OpenRGB Matrix Text Plugin Tab                          |
|                                                           |
|   This file is part of the OpenRGB Matrix Text Plugin     |
|   project                                                 |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "PixelScreenTab.h"
#include "ui_PixelScreenTab.h"
#include "DeviceSettingsPage.h"

PixelScreenTab::PixelScreenTab(PixelScreenPlugin* plugin_ptr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PixelScreenTab),
    plugin(plugin_ptr)
{
    ui->setupUi(this);

    UpdateDeviceList();
}

PixelScreenTab::~PixelScreenTab()
{
    delete ui;
}

void PixelScreenTab::UpdateDeviceList()
{
    ui->deviceTabWidget->clear();

    for (const std::string& display_name : plugin->GetMatrixZoneNames())
    {
        DeviceSettingsPage *page = new DeviceSettingsPage(plugin, display_name, this);
        ui->deviceTabWidget->addTab(page, QString::fromStdString(display_name));
    }
}
