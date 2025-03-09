// comboboxfactory.h
#ifndef COMBOBOXFACTORY_H
#define COMBOBOXFACTORY_H

#include "settingscontrolfactory.h"
#include <QStringList>

class ComboBoxFactory : public SettingsControlFactory {
public:
    ComboBoxFactory(const QStringList& options);
    QWidget* create() const override;
private:
    QStringList options_;
};

#endif // COMBOBOXFACTORY_H
