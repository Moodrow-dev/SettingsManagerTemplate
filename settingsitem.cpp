#include "settingsitem.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>

SettingsItem::SettingsItem(const QString& name, const QString& id,
                           const QString& description, SettingsControlFactory* factory)
    : name_(name), id_(id), description_(description), factory_(factory), controlWidget_(nullptr) {}

SettingsItem::~SettingsItem() {
    delete factory_;
    delete controlWidget_;
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

    controlWidget_ = factory_->create();
    rowLayout->addLayout(leftLayout);
    rowLayout->addWidget(controlWidget_, 1);

    return rowLayout;
}

QWidget* SettingsItem::controlWidget() const {
    return controlWidget_;
}

QVariant SettingsItem::getValue() const {
    if (!controlWidget_) {
        return QVariant();
    }

    if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(controlWidget_)) {
        return lineEdit->text();
    } else if (QComboBox* comboBox = qobject_cast<QComboBox*>(controlWidget_)) {
        return comboBox->currentText();
    } else if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(controlWidget_)) {
        return checkBox->isChecked();
    } else if (QSpinBox* spinBox = qobject_cast<QSpinBox*>(controlWidget_)) {
        return spinBox->value();
    } else if (QWidget* fileBrowse = qobject_cast<QWidget*>(controlWidget_)) {
        // Если это контейнер из FileBrowseFactory, ищем QLineEdit внутри
        QLineEdit* lineEdit = fileBrowse->findChild<QLineEdit*>();
        if (lineEdit) {
            return lineEdit->text();
        }
    }
    // Если тип не поддерживается
    return QVariant();
}
QComboBox* SettingsItem::comboBox() const {
    return qobject_cast<QComboBox*>(controlWidget_);
}

QCheckBox* SettingsItem::checkBox() const {
    return qobject_cast<QCheckBox*>(controlWidget_);
}

QSpinBox* SettingsItem::spinBox() const {
    return qobject_cast<QSpinBox*>(controlWidget_);
}
