#include "spinboxfactory.h"
#include <QSpinBox>

QWidget* SpinBoxFactory::create() const {
    return new QSpinBox();
}
