#ifndef COLORDIALOGFACTORY_H
#define COLORDIALOGFACTORY_H

#include <QWidget>
#include "settingscontrolfactory.h"
#include <QLineEdit>
#include <QPushButton>
#include <QColorDialog>

class ColorDialogFactory : public SettingsControlFactory {
public:
    ColorDialogFactory();
    ~ColorDialogFactory();

    QWidget* create(SettingsItem* settingsItem, const ControlInputCallback& contolInputCallback) const override;
    void setValueToWidget(QWidget* widget, const QVariant& value) const override;
    QVariant getValueFormWidget(QWidget* widget) const override;
};

#endif // COLORDIALOGFACTORY_H
