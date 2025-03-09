// spinboxfactory.h
#ifndef SPINBOXFACTORY_H
#define SPINBOXFACTORY_H

#include "settingscontrolfactory.h"

class SpinBoxFactory : public SettingsControlFactory {
public:
    SpinBoxFactory(int numFrom, int numTo);
    QWidget* create() const override;

private:
    int numFrom_;
    int numTo_;
};

#endif // SPINBOXFACTORY_H
