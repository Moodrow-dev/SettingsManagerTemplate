// checkboxfactory.h
#ifndef CHECKBOXFACTORY_H
#define CHECKBOXFACTORY_H

#include "settingscontrolfactory.h"

class CheckBoxFactory : public SettingsControlFactory {
public:
    QWidget* create() const override;
};

#endif // CHECKBOXFACTORY_H
