#ifndef SETTINGSITEM_H
#define SETTINGSITEM_H

#include "settingscontrolfactory.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>

class SettingsItem {
public:
    SettingsItem(const QString& name, const QString& id,
                 const QString& description, SettingsControlFactory* factory);
    ~SettingsItem();

    QString name() const;
    QString id() const;
    QString description() const;
    SettingsControlFactory* factory() const;;

    QHBoxLayout* createWidget() const;
    QWidget* controlWidget() const; // Новый метод для доступа к виджету

    // Методы для получения конкретных типов виджетов
    QComboBox* comboBox() const;
    QCheckBox* checkBox() const;
    QSpinBox* spinBox() const;
    QVariant getValue() const;

private:
    QString name_;
    QString id_;
    QString description_;
    SettingsControlFactory* factory_;
    mutable QWidget* controlWidget_; // Для хранения созданного виджета
};

#endif // SETTINGSITEM_H
