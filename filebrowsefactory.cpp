#include "filebrowsefactory.h"
#include <QHBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QDir>

FileBrowseFactory::FileBrowseFactory() {}

QWidget* FileBrowseFactory::create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const {
    QWidget* container = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(container);

    QLineEdit* lineEdit = new QLineEdit;
    QPushButton* pushButton = new QPushButton("Browse...");

    layout->addWidget(lineEdit);
    layout->addWidget(pushButton);

    container->setLayout(layout);

    QObject::connect(pushButton, &QPushButton::clicked, pushButton, [lineEdit, settingsItem, contolInputCallback](){
        QString filePath = QFileDialog::getOpenFileName(nullptr, "Выберите файл", QDir::homePath());
        if (!filePath.isEmpty()) {
            lineEdit->setText(filePath);
          contolInputCallback(settingsItem, filePath);
        }
    });

    return container;
}

void FileBrowseFactory::setValueToWidget(QWidget* widget, const QVariant& value) const
{
    QLineEdit* lineEdit = widget->findChild<QLineEdit*>();
    if (lineEdit) {
      lineEdit->setText(value.toString());
      qDebug() << "   ✅ Set to LineEdit (found in child)";
    }
}

QVariant FileBrowseFactory::getValueFormWidget(QWidget* widget) const
{
    QLineEdit* lineEdit = widget->findChild<QLineEdit*>();
    if (lineEdit) {
      return lineEdit->text();
    }

    return QVariant();
}
