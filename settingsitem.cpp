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

SettingsItem::SettingsItem(const QString& name, const QString& id,
                           const QString& description, SettingsControlFactory* factory)
    : name_(name), id_(id), description_(description), factory_(factory) {}

SettingsItem::~SettingsItem() {
    delete factory_;
}

QString SettingsItem::name() const {
    return name_;
}

QString SettingsItem::id() const {
    return id_;
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

    rowLayout->addLayout(leftLayout);
    rowLayout->addWidget(control, 1);

    return rowLayout;
}
