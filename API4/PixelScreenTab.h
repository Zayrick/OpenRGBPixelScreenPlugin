/*---------------------------------------------------------*\
| OpenRGBMatrixTextTab.h                                    |
|                                                           |
|   OpenRGB Matrix Text Plugin Tab                          |
|                                                           |
|   This file is part of the OpenRGB Matrix Text Plugin     |
|   project                                                 |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>
#include "PixelScreenPlugin.h"

namespace Ui {
class PixelScreenTab;
}

class PixelScreenTab : public QWidget
{
    Q_OBJECT

public:
    explicit PixelScreenTab(PixelScreenPlugin* plugin_ptr, QWidget *parent = nullptr);
    ~PixelScreenTab();

public slots:
    void UpdateDeviceList();

private:
    Ui::PixelScreenTab *ui;
    PixelScreenPlugin *plugin;
};
