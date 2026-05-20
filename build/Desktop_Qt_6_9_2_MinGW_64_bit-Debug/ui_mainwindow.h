/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *inputLayout;
    QLabel *inputLabel;
    QLineEdit *inputEdit;
    QLabel *sourceBaseLabel;
    QLineEdit *sourceBaseEdit;
    QLabel *targetBaseLabel;
    QLineEdit *targetBaseEdit;
    QHBoxLayout *buttonLayout;
    QPushButton *convertButton;
    QPushButton *loadButton;
    QPushButton *saveButton;
    QLabel *messageLabel;
    QTextEdit *outputEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(760, 500);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        inputLayout = new QGridLayout();
        inputLayout->setObjectName("inputLayout");
        inputLabel = new QLabel(centralwidget);
        inputLabel->setObjectName("inputLabel");

        inputLayout->addWidget(inputLabel, 0, 0, 1, 1);

        inputEdit = new QLineEdit(centralwidget);
        inputEdit->setObjectName("inputEdit");

        inputLayout->addWidget(inputEdit, 0, 1, 1, 3);

        sourceBaseLabel = new QLabel(centralwidget);
        sourceBaseLabel->setObjectName("sourceBaseLabel");

        inputLayout->addWidget(sourceBaseLabel, 1, 0, 1, 1);

        sourceBaseEdit = new QLineEdit(centralwidget);
        sourceBaseEdit->setObjectName("sourceBaseEdit");

        inputLayout->addWidget(sourceBaseEdit, 1, 1, 1, 1);

        targetBaseLabel = new QLabel(centralwidget);
        targetBaseLabel->setObjectName("targetBaseLabel");

        inputLayout->addWidget(targetBaseLabel, 1, 2, 1, 1);

        targetBaseEdit = new QLineEdit(centralwidget);
        targetBaseEdit->setObjectName("targetBaseEdit");

        inputLayout->addWidget(targetBaseEdit, 1, 3, 1, 1);


        verticalLayout->addLayout(inputLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        convertButton = new QPushButton(centralwidget);
        convertButton->setObjectName("convertButton");

        buttonLayout->addWidget(convertButton);

        loadButton = new QPushButton(centralwidget);
        loadButton->setObjectName("loadButton");

        buttonLayout->addWidget(loadButton);

        saveButton = new QPushButton(centralwidget);
        saveButton->setObjectName("saveButton");

        buttonLayout->addWidget(saveButton);


        verticalLayout->addLayout(buttonLayout);

        messageLabel = new QLabel(centralwidget);
        messageLabel->setObjectName("messageLabel");
        messageLabel->setWordWrap(true);

        verticalLayout->addWidget(messageLabel);

        outputEdit = new QTextEdit(centralwidget);
        outputEdit->setObjectName("outputEdit");
        outputEdit->setReadOnly(true);

        verticalLayout->addWidget(outputEdit);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Universal Converter", nullptr));
        inputLabel->setText(QCoreApplication::translate("MainWindow", "\320\230\321\201\321\205\320\276\320\264\320\275\320\276\320\265 \321\207\320\270\321\201\320\273\320\276:", nullptr));
        inputEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\277\321\200\320\270\320\274\320\265\321\200: 0.[10][20]([30])", nullptr));
        sourceBaseLabel->setText(QCoreApplication::translate("MainWindow", "\320\236\321\201\320\275\320\276\320\262\320\260\320\275\320\270\320\265 p:", nullptr));
        sourceBaseEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "2..500", nullptr));
        targetBaseLabel->setText(QCoreApplication::translate("MainWindow", "\320\236\321\201\320\275\320\276\320\262\320\260\320\275\320\270\320\265 q:", nullptr));
        targetBaseEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "2..500", nullptr));
        convertButton->setText(QCoreApplication::translate("MainWindow", "Convert", nullptr));
        loadButton->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\270\320\267 \321\204\320\260\320\271\320\273\320\260", nullptr));
        saveButton->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \321\200\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202", nullptr));
        messageLabel->setText(QString());
        outputEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\227\320\264\320\265\321\201\321\214 \320\277\320\276\321\217\320\262\320\270\321\202\321\201\321\217 \321\200\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
