/********************************************************************************
** Form generated from reading UI file 'PixelScreenTab.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PIXELSCREENTAB_H
#define UI_PIXELSCREENTAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "CustomTabWidget.h"

QT_BEGIN_NAMESPACE

class Ui_PixelScreenTab
{
public:
    QVBoxLayout *mainLayout;
    CustomTabWidget *deviceTabWidget;

    void setupUi(QWidget *PixelScreenTab)
    {
        if (PixelScreenTab->objectName().isEmpty())
            PixelScreenTab->setObjectName(QString::fromUtf8("PixelScreenTab"));
        PixelScreenTab->resize(750, 500);
        mainLayout = new QVBoxLayout(PixelScreenTab);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        deviceTabWidget = new CustomTabWidget(PixelScreenTab);
        deviceTabWidget->setObjectName(QString::fromUtf8("deviceTabWidget"));
        deviceTabWidget->setTabPosition(QTabWidget::West);

        mainLayout->addWidget(deviceTabWidget);


        retranslateUi(PixelScreenTab);

        deviceTabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(PixelScreenTab);
    } // setupUi

    void retranslateUi(QWidget *PixelScreenTab)
    {
        PixelScreenTab->setWindowTitle(QCoreApplication::translate("PixelScreenTab", "PixelScreen", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PixelScreenTab: public Ui_PixelScreenTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PIXELSCREENTAB_H
