#ifndef LINEEDITFACTORY_H
#define LINEEDITFACTORY_H

#include "settingscontrolfactory.h"

class LineEditFactory : public SettingsControlFactory
{
public:
    LineEditFactory();
    QWidget* create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const override;
    void setValueToWidget(QWidget* widget, const QVariant& value) const override;
    QVariant getValueFormWidget(QWidget* widget) const override;
};

#endif
