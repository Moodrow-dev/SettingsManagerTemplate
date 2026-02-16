#include "settingsitem.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

// Конструктор для обычных настроек
SettingsItem::SettingsItem(SettingsItem* parent, const QString& name, const QString& id,
                           const QString& description, SettingsControlFactory* factory,
                           bool enableSaving)
    : parent_(parent), name_(name), id_(id), description_(description),
      factory_(factory), enableSaving_(enableSaving)
{
    if (parent_) {
        parent_->appendChild(this);
    }
}

// Конструктор для групп
SettingsItem::SettingsItem(SettingsItem* parent, const QString& name, const QString& id,
                           const QString& description)
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

QString SettingsItem::value() const {
    return value_;
}

void SettingsItem::setValue(const QVariant& value) {
    value_ = value.toString();
}

SettingsControlFactory* SettingsItem::factory() const {
    return factory_;
}

QHBoxLayout* SettingsItem::createWidget() {
    if (isGroup()) {
        return nullptr;
    }

    QHBoxLayout* rowLayout = new QHBoxLayout();
    QVBoxLayout* leftLayout = new QVBoxLayout();
    QLabel* nameLabel = new QLabel(name_);
    QLabel* hintLabel = new QLabel(description_);
    leftLayout->addWidget(nameLabel);
    leftLayout->addWidget(hintLabel);

    // Создаем виджет
    QWidget* controlWidget = factory_->create();
    rowLayout->addLayout(leftLayout);
    rowLayout->addWidget(controlWidget, 1);

    return rowLayout;
}

// Новый метод для получения значения из виджета
QVariant SettingsItem::getValueFromWidget(QWidget* controlWidget) const {
    if (!controlWidget || isGroup()) {
        return QVariant();
    }

    if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(controlWidget)) {
        return lineEdit->text();
    } else if (QComboBox* comboBox = qobject_cast<QComboBox*>(controlWidget)) {
        return comboBox->currentText();
    } else if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(controlWidget)) {
        return checkBox->isChecked();
    } else if (QSpinBox* spinBox = qobject_cast<QSpinBox*>(controlWidget)) {
        return spinBox->value();
    } else if (QWidget* fileBrowse = qobject_cast<QWidget*>(controlWidget)) {
        QLineEdit* lineEdit = fileBrowse->findChild<QLineEdit*>();
        if (lineEdit) {
            return lineEdit->text();
        }
    }

    return QVariant();
}

bool SettingsItem::isSavingEnabled() const {
    return enableSaving_ && !isGroup();
}

void SettingsItem::setValueToWidget(QWidget* controlWidget, const QVariant& value) const {
    if (!controlWidget || isGroup()) {
        qDebug() << "⚠️ setValueToWidget: no widget or group";
        return;
    }

    qDebug() << "🔧 Setting value to widget:" << name() << "value:" << value;

    if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(controlWidget)) {
        lineEdit->setText(value.toString());
        qDebug() << "   ✅ Set to LineEdit";
    } else if (QComboBox* comboBox = qobject_cast<QComboBox*>(controlWidget)) {
        int index = comboBox->findText(value.toString());
        if (index >= 0) {
            comboBox->setCurrentIndex(index);
            qDebug() << "   ✅ Set to ComboBox at index" << index;
        }
    } else if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(controlWidget)) {
        checkBox->setChecked(value.toBool());
        qDebug() << "   ✅ Set to CheckBox";
    } else if (QSpinBox* spinBox = qobject_cast<QSpinBox*>(controlWidget)) {
        spinBox->setValue(value.toInt());
        qDebug() << "   ✅ Set to SpinBox";
    } else {
        QLineEdit* lineEdit = controlWidget->findChild<QLineEdit*>();
        if (lineEdit) {
            lineEdit->setText(value.toString());
            qDebug() << "   ✅ Set to LineEdit (found in child)";
        }
    }
}