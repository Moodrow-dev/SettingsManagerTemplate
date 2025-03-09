// settingsitem.h
#ifndef SETTINGSITEM_H
#define SETTINGSITEM_H

#include <QVariant>
#include <QString>
#include "settingscontrolfactory.h"

class QHBoxLayout; // Форвард-декларация, чтобы не включать лишние заголовки

class SettingsItem {
public:
    SettingsItem(const QString& name, const QString& id,
                 const QString& description, SettingsControlFactory* factory);
    ~SettingsItem();

    QString name() const;
    QString id() const;
    QString description() const;
    SettingsControlFactory* factory() const;

    // Новый метод для создания макета элемента
    QHBoxLayout* createWidget() const;

private:
    QString name_;
    QString id_;
    QString description_;
    SettingsControlFactory* factory_;
};

#endif // SETTINGSITEM_H
