#include "colordialogfactory.h"
#include <QHBoxLayout>
#include <QColorDialog>
#include <QColor>
#include <QLabel>

ColorDialogFactory::ColorDialogFactory() {}

ColorDialogFactory::~ColorDialogFactory() {
}

QWidget* ColorDialogFactory::create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const {
    QWidget* container = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(container);


    QLineEdit* lineEdit = new QLineEdit;
    QPushButton* pushButton = new QPushButton("Choose color...");

    layout->addWidget(lineEdit);
    layout->addWidget(pushButton);


    QAbstractButton::connect(pushButton, &QPushButton::clicked, pushButton, [lineEdit, settingsItem, contolInputCallback]() {
        QColorDialog dialog;
        dialog.setOptions(QColorDialog::ShowAlphaChannel); // Показываем альфа-канал
        dialog.setCurrentColor(QColor(lineEdit->text())); // Устанавливаем текущий цвет из QLineEdit

        if (dialog.exec() == QColorDialog::Accepted) {
            QColor selectedColor = dialog.currentColor();
            QString colorPattern = selectedColor.name(QColor::HexArgb).toUpper(); // Получаем HEX с альфа-каналом
            lineEdit->setText(colorPattern);
            contolInputCallback(settingsItem, colorPattern);
        }
    });

    container->setLayout(layout);
    return container;
}

void ColorDialogFactory::setValueToWidget(QWidget* widget, const QVariant& value) const
{
    QLineEdit* lineEdit = widget->findChild<QLineEdit*>();
    if (lineEdit) {
      lineEdit->setText(value.toString());
      qDebug() << "   ✅ Set to LineEdit (found in child)";
    }
}

QVariant ColorDialogFactory::getValueFormWidget(QWidget* widget) const
{
    QLineEdit* lineEdit = widget->findChild<QLineEdit*>();
    if (lineEdit) {
        return lineEdit->text();
    }

    return QVariant();
}
