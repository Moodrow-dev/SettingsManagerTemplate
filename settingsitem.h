#ifndef SETTINGSITEM_H
#define SETTINGSITEM_H

#include "settingscontrolfactory.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QString>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

class SettingsItem {
public:
    SettingsItem(const QString& name, const QString& id, const QString& description,
                 SettingsControlFactory* factory, bool enableSaving = true,
                 const QString& groupName = "General");

    ~SettingsItem();

    QString name() const;
    QString id() const;
    QString description() const;
    SettingsControlFactory* factory() const;

    QHBoxLayout* createWidget() const;
    QWidget* controlWidget() const;
    QVariant getValue() const;

    // Новые методы для работы с сохранением и группами
    bool isSavingEnabled() const;
    QString groupName() const;

    QComboBox* comboBox() const;
    QCheckBox* checkBox() const;
    QSpinBox* spinBox() const;

private:
    QString name_;
    QString id_;
    QString description_;
    SettingsControlFactory* factory_;
    mutable QWidget* controlWidget_;
    bool enableSaving_;              // Флаг сохранения
    QString groupName_;              // Группа (вкладка)
};

#endif // SETTINGSITEM_H
