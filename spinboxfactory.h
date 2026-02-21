#ifndef SPINBOXFACTORY_H
#define SPINBOXFACTORY_H

#include "settingscontrolfactory.h"

class SpinBoxFactory : public SettingsControlFactory {
public:
    SpinBoxFactory(const int numFrom, const int numTo);
    QWidget* create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const override;
    void setValueToWidget(QWidget* widget, const QVariant& value) const override;
    QVariant getValueFormWidget(QWidget* widget) const override;

private:
    int numFrom_;
    int numTo_;
};

#endif
