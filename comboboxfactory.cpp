#include "comboboxfactory.h"
#include <QComboBox>

ComboBoxFactory::ComboBoxFactory(const QStringList& options) : options_(options) {}

QWidget* ComboBoxFactory::create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const {
    QComboBox* comboBox = new QComboBox();
    comboBox->addItems(options_);
    QObject::connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), comboBox,
        [settingsItem, contolInputCallback](const int index) {
            contolInputCallback(settingsItem, index);
        });
    return comboBox;
}

void ComboBoxFactory::setValueToWidget(QWidget* widget, const QVariant& value) const
{
    if (QComboBox* comboBox = qobject_cast<QComboBox*>(widget)) {
        int index = comboBox->findText(value.toString());
        if (index >= 0) {
            comboBox->setCurrentIndex(index);
            qDebug() << "   ✅ Set to ComboBox at index" << index;
        }
    }
}

QVariant ComboBoxFactory::getValueFormWidget(QWidget* widget) const
{
    if (QComboBox* comboBox = qobject_cast<QComboBox*>(widget))
        return comboBox->currentText();

    return QVariant();
}
