#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "BaseConverter.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->convertButton, &QPushButton::clicked, this, &MainWindow::onConvertClicked);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);

    ui->messageLabel->setText("Введите число и основания систем счисления.");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onConvertClicked() {
    std::string input = ui->inputEdit->text().trimmed().toStdString();
    std::string p = ui->sourceBaseEdit->text().trimmed().toStdString();
    std::string q = ui->targetBaseEdit->text().trimmed().toStdString();

    ConvertResult result = BaseConverter::convert(input, p, q);

    ui->outputEdit->setPlainText(QString::fromStdString(result.output));
    ui->messageLabel->setText(QString::fromStdString(result.message));
}

void MainWindow::onLoadClicked() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Открыть входной файл",
                                                    QString(),
                                                    "Text files (*.txt);;All files (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    QTextStream in(&file);
    QString p = in.readLine().trimmed();
    QString q = in.readLine().trimmed();
    QString input = in.readLine().trimmed();

    if (p.isEmpty() || q.isEmpty() || input.isEmpty()) {
        QMessageBox::warning(this,
                             "Ошибка", "Файл должен содержать три строки: p, q и input.");
        return;
    }

    ui->sourceBaseEdit->setText(p);
    ui->targetBaseEdit->setText(q);
    ui->inputEdit->setText(input);
    ui->messageLabel->setText("Данные загружены из файла.");
}

void MainWindow::onSaveClicked() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Сохранить результат",
                                                    QString(),
                                                    "Text files (*.txt);;All files (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл.");
        return;
    }

    QTextStream out(&file);
    out << ui->outputEdit->toPlainText() << '\n';
    out << ui->messageLabel->text() << '\n';

    ui->messageLabel->setText("Результат сохранён в файл.");
}
