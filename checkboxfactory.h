#ifndef CHECKBOXFACTORY_H
#define CHECKBOXFACTORY_H

#include "settingscontrolfactory.h"

class CheckBoxFactory : public SettingsControlFactory {
public:
    CheckBoxFactory();

    QWidget* create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const override;
    void setValueToWidget(QWidget* widget, const QVariant& value) const override;
    QVariant getValueFormWidget(QWidget* widget) const override;
};

#endif
