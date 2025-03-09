#include "checkboxfactory.h"
#include <QCheckBox>

QWidget* CheckBoxFactory::create() const {
    return new QCheckBox();
}
