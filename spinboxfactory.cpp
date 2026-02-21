#include "spinboxfactory.h"
#include <QSpinBox>
#include <algorithm>

#include "settingsitem.h"

SpinBoxFactory::SpinBoxFactory(int numFrom, int numTo)
    : numFrom_(numFrom), numTo_(numTo) {}

QWidget* SpinBoxFactory::create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const {
    QSpinBox* spinBox = new QSpinBox();

    int minVal = std::min(numFrom_, numTo_);
    int maxVal = std::max(numFrom_, numTo_);
    spinBox->setRange(minVal, maxVal);

    QObject::connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), spinBox, [settingsItem, contolInputCallback](const int value) {
        contolInputCallback(settingsItem, value);
    });

    return spinBox;
}

void SpinBoxFactory::setValueToWidget(QWidget* widget, const QVariant& value) const
{
    if (QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget)) {
        spinBox->setValue(value.toInt());
        qDebug() << "   ✅ Set to SpinBox";
    }
}

QVariant SpinBoxFactory::getValueFormWidget(QWidget* widget) const
{
    if (QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget))
        return spinBox->value();

    return QVariant();
}
