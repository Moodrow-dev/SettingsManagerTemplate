#ifndef SETTINGSCONTROLFACTORY_H
#define SETTINGSCONTROLFACTORY_H

#include <QWidget>

class SettingsAccess;
class SettingsItem;

class SettingsControlFactory {
public:
    using ControlInputCallback = std::function<void(SettingsItem* settingsItem, const QVariant& value)>;

    virtual ~SettingsControlFactory() = default;

    virtual QWidget* create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const = 0;
    virtual void setValueToWidget(QWidget* widget, const QVariant& value) const  = 0;
    virtual QVariant getValueFormWidget(QWidget* widget) const = 0;
};

#endif
