#ifndef SETTINGSITEM_H
#define SETTINGSITEM_H

#include <QVariant>
#include <QString>
#include "settingscontrolfactory.h"

class SettingsItem {
public:
    SettingsItem(const QString& name, const QString& id, const QVariant& defaultValue,
                 const QString& description, SettingsControlFactory* factory);
    ~SettingsItem();

    QString name() const;
    QString id() const;
    QVariant defaultValue() const;
    QString description() const;
    SettingsControlFactory* factory() const;

private:
    QString name_;              // Название настройки
    QString id_;                // Идентификатор
    QVariant defaultValue_;     // Значение по умолчанию (может быть строкой, числом, булевым значением)
    QString description_;       // Описание или подсказка
    SettingsControlFactory* factory_; // Указатель на фабрику
};

#endif
