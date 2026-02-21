#ifndef COMBOBOXFACTORY_H
#define COMBOBOXFACTORY_H

#include "settingscontrolfactory.h"
#include <QStringList>

class ComboBoxFactory : public SettingsControlFactory {
public:
    ComboBoxFactory(const QStringList& options);
    QWidget* create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const override;
    void setValueToWidget(QWidget* widget, const QVariant& value) const override;
    QVariant getValueFormWidget(QWidget* widget) const override;

private:
    QStringList options_;
};

#endif
