#include "settingsitem.h"

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
