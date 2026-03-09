#ifndef SETTINGSITEM_H
#define SETTINGSITEM_H

#include "settingscontrolfactory.h"
#include <QHBoxLayout>
#include <QString>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QList>

class SettingsItem {
public:
    // Конструктор для обычных настроек
    SettingsItem(const QString& id, const QString& name, const QString& description, const QVariant& defaultValue,
                 SettingsItem* parent = nullptr, SettingsControlFactory* factory = nullptr,
                 bool enableSaving = true);

    // Конструктор для групп (без factory и enableSaving)
    SettingsItem(const QString& id, const QString& name, const QString& description, SettingsItem* parent = nullptr);

    ~SettingsItem();

    // Методы для древовидной структуры
    SettingsItem* parent() const;
    void appendChild(SettingsItem* child);
    SettingsItem* child(int row);
    int childCount() const;
    int row() const;

    // Группа - это элемент без родителя ИЛИ элемент без factory
    bool isGroup() const {
        return (parent_ == nullptr) || (factory_ == nullptr);
    }

    QList<SettingsItem*> getAllChildren() const;
    SettingsItem* findItemById(const QString& id) const;
    SettingsItem* findItemByName(const QString& name) const;

    // Основные методы
    QString name() const;
    QString id() const;
    QString description() const;
    QVariant defaultValue() const;
    SettingsControlFactory* factory() const;

    bool isSavingEnabled() const;

private:
    SettingsItem* parent_;
    QList<SettingsItem*> childItems_;
    QString name_;
    QString id_;
    QString description_;
    QVariant defaultValue_;
    SettingsControlFactory* factory_;
    bool enableSaving_;

};

#endif // SETTINGSITEM_H
