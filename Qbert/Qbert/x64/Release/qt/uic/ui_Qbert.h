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

QT_BEGIN_NAMESPACE

class Ui_QbertClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QbertClass)
    {
        if (QbertClass->objectName().isEmpty())
            QbertClass->setObjectName("QbertClass");
        QbertClass->resize(600, 400);
        menuBar = new QMenuBar(QbertClass);
        menuBar->setObjectName("menuBar");
        QbertClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QbertClass);
        mainToolBar->setObjectName("mainToolBar");
        QbertClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(QbertClass);
        centralWidget->setObjectName("centralWidget");
        QbertClass->setCentralWidget(centralWidget);
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
