/********************************************************************************
** Form generated from reading UI file 'Qbert.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QBERT_H
#define UI_QBERT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <qbert.h>
#include "CubeWidget.h"

QT_BEGIN_NAMESPACE

class Ui_QbertClass
{
public:
    QWidget *centralWidget;
    CubeWidget *widget;
    Qbert *qbert;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QbertClass)
    {
        if (QbertClass->objectName().isEmpty())
            QbertClass->setObjectName("QbertClass");
        QbertClass->resize(2000, 1200);
        centralWidget = new QWidget(QbertClass);
        centralWidget->setObjectName("centralWidget");
        widget = new CubeWidget(centralWidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(100, 100, 100, 100));
        widget->setAutoFillBackground(false);
        qbert = new Qbert(centralWidget);
        qbert->setObjectName("qbert");
        qbert->setGeometry(QRect(340, 170, 500, 500));
        QbertClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QbertClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 2000, 21));
        QbertClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QbertClass);
        mainToolBar->setObjectName("mainToolBar");
        QbertClass->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QbertClass);
        statusBar->setObjectName("statusBar");
        QbertClass->setStatusBar(statusBar);

        retranslateUi(QbertClass);

        QMetaObject::connectSlotsByName(QbertClass);
    } // setupUi

    void retranslateUi(QMainWindow *QbertClass)
    {
        QbertClass->setWindowTitle(QCoreApplication::translate("QbertClass", "Qbert", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QbertClass: public Ui_QbertClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QBERT_H
