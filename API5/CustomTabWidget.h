/*---------------------------------------------------------*\
| CustomTabWidget.h                                         |
|                                                           |
|   QTabWidget subclass exposing setTabBar to install       |
|   HorizontalTabBar for West side tabs.                    |
|                                                           |
|   This file is part of the OpenRGB Matrix Text Plugin     |
|   project                                                 |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QTabWidget>
#include "HorizontalTabBar.h"

class CustomTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit CustomTabWidget(QWidget* parent = nullptr) : QTabWidget(parent)
    {
        setTabBar(new HorizontalTabBar(this));
    }
};
