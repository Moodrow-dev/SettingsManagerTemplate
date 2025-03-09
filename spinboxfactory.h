#ifndef SPINBOXFACTORY_H
#define SPINBOXFACTORY_H

#include "settingscontrolfactory.h"

class SpinBoxFactory : public SettingsControlFactory {
public:
    QWidget* create() const override;
};

#endif
