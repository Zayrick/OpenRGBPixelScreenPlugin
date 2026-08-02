/*---------------------------------------------------------*\
| HorizontalTabBar.h                                        |
|                                                           |
|   Custom QTabBar for West-positioned side tabs with      |
|   un-rotated left-aligned horizontal text.                |
|                                                           |
|   This file is part of the OpenRGB Matrix Text Plugin     |
|   project                                                 |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <QTabBar>
#include <QStylePainter>
#include <QStyleOptionTab>

class HorizontalTabBar : public QTabBar
{
    Q_OBJECT

public:
    explicit HorizontalTabBar(QWidget* parent = nullptr) : QTabBar(parent) {}

    QSize tabSizeHint(const int index) const override
    {
        QSize s = QTabBar::tabSizeHint(index);
        s.transpose();
        s.setWidth(qMax(s.width(), 180));
        s.setHeight(qMax(s.height(), 35));
        return s;
    }

protected:
    void paintEvent(QPaintEvent* /*event*/) override
    {
        QStylePainter painter(this);
        QStyleOptionTab opt;

        for (int i = 0; i < count(); i++)
        {
            initStyleOption(&opt, i);
            painter.drawControl(QStyle::CE_TabBarTabShape, opt);

            QStyleOptionTab tab = opt;
            tab.rect = tabRect(i);

            // Left-align text inside side tabs with padding
            QRect textRect = tab.rect.adjusted(12, 0, -8, 0);
            painter.drawItemText(textRect, Qt::AlignLeft | Qt::AlignVCenter, palette(), isTabEnabled(i), tab.text);
        }
    }
};
