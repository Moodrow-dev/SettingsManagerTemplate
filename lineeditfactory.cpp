#include "lineeditfactory.h"
#include <QLineEdit>

LineEditFactory::LineEditFactory() {}

QWidget* LineEditFactory::create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const {
    QLineEdit* lineEdit = new QLineEdit;
    QObject::connect(lineEdit, &QLineEdit::textChanged, lineEdit, [settingsItem, contolInputCallback](const QString& value) {
      contolInputCallback(settingsItem, value);
    });
    return lineEdit;
}

void LineEditFactory::setValueToWidget(QWidget* widget, const QVariant& value) const
{
    if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget)) {
        lineEdit->setText(value.toString());
        qDebug() << "   ✅ Set to LineEdit";
    }
}

QVariant LineEditFactory::getValueFormWidget(QWidget* widget) const
{
    if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget))
        return lineEdit->text();

    return QVariant();
}
