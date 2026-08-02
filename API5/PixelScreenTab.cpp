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

    std::shared_lock<std::shared_mutex> lock(plugin->matrix_zones_mutex);

    for (const auto& target : plugin->matrix_zones)
    {
        DeviceSettingsPage *page = new DeviceSettingsPage(plugin, target.display_name, this);
        ui->deviceTabWidget->addTab(page, QString::fromStdString(target.display_name));
    }
}
