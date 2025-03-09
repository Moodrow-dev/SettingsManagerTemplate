// spinboxfactory.cpp
#include "spinboxfactory.h"
#include <QSpinBox>
#include <algorithm>

SpinBoxFactory::SpinBoxFactory(int defaultValue, int numFrom, int numTo)
    : defaultValue_(defaultValue), numFrom_(numFrom), numTo_(numTo) {}

QWidget* SpinBoxFactory::create() const {
    QSpinBox* spinBox = new QSpinBox();
    // Убеждаемся, что минимальное значение меньше максимального
    int minVal = std::min(numFrom_, numTo_);
    int maxVal = std::max(numFrom_, numTo_);
    spinBox->setRange(minVal, maxVal);
    spinBox->setValue(defaultValue_);
    return spinBox;
}
