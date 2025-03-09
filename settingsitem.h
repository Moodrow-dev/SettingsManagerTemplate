// settingsitem.h
#ifndef SETTINGSITEM_H
#define SETTINGSITEM_H

#include <QVariant>
#include <QString>
#include "settingscontrolfactory.h"

class QHBoxLayout;

class SettingsItem {
public:
    SettingsItem(const QString& name, const QString& id,
                 const QString& description, SettingsControlFactory* factory);
    ~SettingsItem();

    QString name() const;
    QString id() const;
    QString description() const;
    SettingsControlFactory* factory() const;

    QHBoxLayout* createWidget() const;

private:
    QString name_;
    QString id_;
    QString description_;
    SettingsControlFactory* factory_;
};

#endif // SETTINGSITEM_H
