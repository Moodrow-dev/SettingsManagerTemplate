// settingsitem.cpp
#include "settingsitem.h"
#include <QHBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include "comboboxfactory.h"
#include "spinboxfactory.h"
#include "checkboxfactory.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>

SettingsItem::SettingsItem(const QString& name, const QString& id, const QVariant& defaultValue,
                           const QString& description, SettingsControlFactory* factory)
    : name_(name), id_(id), defaultValue_(defaultValue), description_(description), factory_(factory) {}

SettingsItem::~SettingsItem() {
    delete factory_;
}

QString SettingsItem::name() const {
    return name_;
}

QString SettingsItem::id() const {
    return id_;
}

QVariant SettingsItem::defaultValue() const {
    return defaultValue_;
}

QString SettingsItem::description() const {
    return description_;
}

SettingsControlFactory* SettingsItem::factory() const {
    return factory_;
}

QHBoxLayout* SettingsItem::createWidget() const {
    QHBoxLayout* rowLayout = new QHBoxLayout();

    QVBoxLayout* leftLayout = new QVBoxLayout();
    QLabel* nameLabel = new QLabel(name_);
    QLabel* hintLabel = new QLabel(description_);
    hintLabel->setStyleSheet("color: gray;");
    leftLayout->addWidget(nameLabel);
    leftLayout->addWidget(hintLabel);

    QWidget* control = factory_->create();

    if (QComboBox* comboBox = qobject_cast<QComboBox*>(control)) {
        comboBox->setCurrentText(defaultValue_.toString());
    } else if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(control)) {
        checkBox->setChecked(defaultValue_.toBool());
    } else if (QSpinBox* spinBox = qobject_cast<QSpinBox*>(control)) {
        spinBox->setRange(0, 1000);
        spinBox->setValue(defaultValue_.toInt());
    }

    rowLayout->addLayout(leftLayout);
    rowLayout->addWidget(control, 1);

    return rowLayout;
}
