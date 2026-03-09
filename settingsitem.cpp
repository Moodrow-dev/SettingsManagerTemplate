#include "settingsitem.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

// Конструктор для обычных настроек
SettingsItem::SettingsItem(const QString& id, const QString& name,
                           const QString& description, const QVariant& defaultValue, SettingsItem* parent, SettingsControlFactory* factory,
                           bool enableSaving)
    : parent_(parent), name_(name), id_(id), description_(description), defaultValue_(defaultValue),
      factory_(factory), enableSaving_(enableSaving)
{
    if (parent_) {
        parent_->appendChild(this);
    }
}

// Конструктор для групп
SettingsItem::SettingsItem(const QString& id, const QString& name,
                           const QString& description, SettingsItem* parent)
    : parent_(parent), name_(name), id_(id), description_(description),
      factory_(nullptr), enableSaving_(false)
{
    if (parent_) {
        parent_->appendChild(this);
    }
}

SettingsItem::~SettingsItem() {
    delete factory_;
    qDeleteAll(childItems_);
}

// Методы для древовидной структуры
SettingsItem* SettingsItem::parent() const {
    return parent_;
}

void SettingsItem::appendChild(SettingsItem* child) {
    childItems_.append(child);
}

SettingsItem* SettingsItem::child(int row) {
    if (row < 0 || row >= childItems_.size())
        return nullptr;
    return childItems_.at(row);
}

int SettingsItem::childCount() const {
    return childItems_.count();
}

int SettingsItem::row() const {
    if (parent_)
        return parent_->childItems_.indexOf(const_cast<SettingsItem*>(this));
    return 0;
}

QList<SettingsItem*> SettingsItem::getAllChildren() const {
    QList<SettingsItem*> allChildren;
    for (SettingsItem* child : childItems_) {
        allChildren.append(child);
        allChildren.append(child->getAllChildren());
    }
    return allChildren;
}

SettingsItem* SettingsItem::findItemById(const QString& id) const {
    if (id_ == id) return const_cast<SettingsItem*>(this);

    for (SettingsItem* child : childItems_) {
        SettingsItem* found = child->findItemById(id);
        if (found) return found;
    }

    return nullptr;
}

SettingsItem* SettingsItem::findItemByName(const QString& name) const {
    if (name_ == name) return const_cast<SettingsItem*>(this);

    for (SettingsItem* child : childItems_) {
        SettingsItem* found = child->findItemByName(name);
        if (found) return found;
    }

    return nullptr;
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

QVariant SettingsItem::defaultValue() const {
    return defaultValue_;
}

SettingsControlFactory* SettingsItem::factory() const {
    return factory_;
}

bool SettingsItem::isSavingEnabled() const {
    return enableSaving_ && !isGroup();
}
