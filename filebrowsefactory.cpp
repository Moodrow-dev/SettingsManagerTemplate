#include "filebrowsefactory.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QDir>

FileBrowseFactory::FileBrowseFactory(LineEditFactory* lineEdit, PushButtonFactory* pushButton)
    : lineEdit_(lineEdit), pushButton_(pushButton), lineEditWidget_(nullptr), pushButtonWidget_(nullptr) {}

QWidget* FileBrowseFactory::create() const {
    QWidget* container = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(container);

    // Используем приведение типов
    lineEditWidget_ = qobject_cast<QLineEdit*>(lineEdit_->create());
    pushButtonWidget_ = qobject_cast<QPushButton*>(pushButton_->create());

    layout->addWidget(lineEditWidget_);
    layout->addWidget(pushButtonWidget_);

    container->setLayout(layout);

    QAbstractButton::connect(pushButtonWidget_, &QPushButton::clicked, [this](){
        QString filePath = QFileDialog::getOpenFileName(nullptr, "Выберите файл", QDir::homePath());
        if (!filePath.isEmpty()) {
            lineEditWidget_->setText(filePath);
        }
    });

    return container;
}
