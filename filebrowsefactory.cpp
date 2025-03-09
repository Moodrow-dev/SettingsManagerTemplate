#include "filebrowsefactory.h"
#include <QHBoxLayout>
#include <QWidget>

FileBrowseFactory::FileBrowseFactory(LineEditFactory* lineEdit, PushButtonFactory* pushButton)
    : lineEdit_(lineEdit), pushButton_(pushButton) {}

QWidget* FileBrowseFactory::create() const {
    QWidget* container = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(container);

    layout->addWidget(lineEdit_->create());
    layout->addWidget(pushButton_->create());

    container->setLayout(layout);

    return container;
}
