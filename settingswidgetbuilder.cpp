#include "settingswidgetbuilder.h"
#include "settingsitem.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QScrollArea>
#include <QLineEdit>
#include <QSettings>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

SettingsWidgetBuilder::SettingsWidgetBuilder(QList<SettingsItem*> widgetList, QObject* parent)
    : QObject(parent), widgetList_(widgetList) {

    QStringList groups;
    QListWidget* sidePanel = new QListWidget();
    QStackedWidget* stackedWidget = new QStackedWidget();

    QMap<QString, QWidget*> pageMap;

    for (SettingsItem* item : std::as_const(widgetList_)) {
        QString group = item->groupName();

        if (!groups.contains(group)) {
            groups.append(group);
            sidePanel->addItem(group);

            QScrollArea* scrollArea = createScrollAreaForGroup(group, widgetList_);
            if (scrollArea) {
                stackedWidget->addWidget(scrollArea);
                pageMap[group] = scrollArea;
            }
        }
    }

    connect(sidePanel, &QListWidget::currentRowChanged, stackedWidget, &QStackedWidget::setCurrentIndex);

    QWidget* containerWidget = new QWidget();
    embedLayout_ = new QHBoxLayout(containerWidget);

    embedLayout_->addWidget(sidePanel, 1);
    embedLayout_->addWidget(stackedWidget, 3);

    loadSettings(); // Загрузка настроек
    connectSignalsForAutoSave(); // Подключение сигналов для автосохранения
}

SettingsWidgetBuilder::~SettingsWidgetBuilder() {
    delete embedLayout_; // Освобождаем ресурсы
}

QWidget* SettingsWidgetBuilder::getEmbeddedWidget() const {
    return embedLayout_->parentWidget();
}

QScrollArea* SettingsWidgetBuilder::createScrollAreaForGroup(const QString& group, const QList<SettingsItem*>& widgetList) {
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);

    QWidget* contentWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(contentWidget);

    for (SettingsItem* item : std::as_const(widgetList)) {
        if (item->groupName() == group) {
            layout->addLayout(item->createWidget());
        }
    }

    scrollArea->setWidget(contentWidget);
    return scrollArea;
}

void SettingsWidgetBuilder::loadSettings() {
    QSettings settings("TestLabs", "TestSettings");

    for (SettingsItem* item : std::as_const(widgetList_)) {
        if (!item->isSavingEnabled()) continue;

        settings.beginGroup(item->groupName());
        QVariant value = settings.value(item->id(), item->getValue());
        if (value.isValid()) {
            if (QComboBox* comboBox = item->comboBox()) {
                comboBox->setCurrentText(value.toString());
            } else if (QCheckBox* checkBox = item->checkBox()) {
                checkBox->setChecked(value.toBool());
            } else if (QSpinBox* spinBox = item->spinBox()) {
                spinBox->setValue(value.toInt());
            } else if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(item->controlWidget())) {
                lineEdit->setText(value.toString());
            } else if (QWidget* compositeWidget = item->controlWidget()) {
                QLineEdit* lineEdit = compositeWidget->findChild<QLineEdit*>();
                if (lineEdit) {
                    lineEdit->setText(value.toString());
                }
            }
        }
        settings.endGroup();
    }
}

void SettingsWidgetBuilder::saveSettings() {
    QSettings settings("TestLabs", "TestSettings");

    for (SettingsItem* item : std::as_const(widgetList_)) {
        if (!item->isSavingEnabled()) continue;

        settings.beginGroup(item->groupName());
        settings.setValue(item->id(), item->getValue());
        settings.endGroup();
    }
}

void SettingsWidgetBuilder::connectSignalsForAutoSave() {
    for (SettingsItem* item : std::as_const(widgetList_)) {
        if (!item->isSavingEnabled()) continue;

        if (QComboBox* comboBox = item->comboBox()) {
            connect(comboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
                    this, &SettingsWidgetBuilder::saveSettings);
        } else if (QCheckBox* checkBox = item->checkBox()) {
            connect(checkBox, &QCheckBox::toggled, this, &SettingsWidgetBuilder::saveSettings);
        } else if (QSpinBox* spinBox = item->spinBox()) {
            connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SettingsWidgetBuilder::saveSettings);
        } else if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(item->controlWidget())) {
            connect(lineEdit, &QLineEdit::textChanged, this, &SettingsWidgetBuilder::saveSettings);
        } else if (QWidget* compositeWidget = item->controlWidget()) {
            QLineEdit* lineEdit = compositeWidget->findChild<QLineEdit*>();
            if (lineEdit) {
                connect(lineEdit, &QLineEdit::textChanged, this, &SettingsWidgetBuilder::saveSettings);
            }
        }
    }
}
