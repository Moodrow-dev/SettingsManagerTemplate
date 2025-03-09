// spinboxfactory.cpp
#include "spinboxfactory.h"
#include <QSpinBox>
#include <algorithm>

SpinBoxFactory::SpinBoxFactory(int numFrom, int numTo)
    : numFrom_(numFrom), numTo_(numTo) {}

QWidget* SpinBoxFactory::create() const {
    QSpinBox* spinBox = new QSpinBox();
    // Убеждаемся, что минимальное значение меньше максимального
    int minVal = std::min(numFrom_, numTo_);
    int maxVal = std::max(numFrom_, numTo_);
    spinBox->setRange(minVal, maxVal);
    return spinBox;
}
