#ifndef FILEBROWSEFACTORY_H
#define FILEBROWSEFACTORY_H

#include <QWidget>
#include "settingscontrolfactory.h"
#include <QLineEdit>
#include <QPushButton>

class FileBrowseFactory : public SettingsControlFactory {
public:
    FileBrowseFactory();
    QWidget* create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const override;
    void setValueToWidget(QWidget* widget, const QVariant& value) const override;
    QVariant getValueFormWidget(QWidget* widget) const override;
};

#endif
