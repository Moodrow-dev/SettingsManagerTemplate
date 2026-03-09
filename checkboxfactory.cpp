#include "checkboxfactory.h"
#include <QCheckBox>

#include "settingsitem.h"

CheckBoxFactory::CheckBoxFactory() {}

QWidget* CheckBoxFactory::create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const {
    QCheckBox* checkbox = new QCheckBox;
    QObject::connect(checkbox, &QCheckBox::toggled, checkbox, [settingsItem, contolInputCallback](const int value) {
      contolInputCallback(settingsItem, value);
    });
    return checkbox;
}

void CheckBoxFactory::setValueToWidget(QWidget* widget, const QVariant& value) const
{
    if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(widget)) {
        checkBox->setChecked(value.toBool());
        qDebug() << "   ✅ Set to CheckBox";
    }
}

QVariant CheckBoxFactory::getValueFormWidget(QWidget* widget) const
{
    if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(widget))
        return checkBox->isChecked();

    return QVariant();
}
