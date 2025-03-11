#include "settingswindow.h"
#include "settingsitem.h"
#include "comboboxfactory.h"
#include "spinboxfactory.h"
#include "checkboxfactory.h"
#include "lineeditfactory.h"
#include "pushbuttonfactory.h"
#include "filebrowsefactory.h"
#include <QVBoxLayout>
#include <QTabWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QSettings>
#include <QMap>

SettingsWindow::SettingsWindow(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QTabWidget* tabWidget = new QTabWidget(this);

    // Теперь с указанием группы и флага сохранения
    items << new SettingsItem("Language", "1", "Select interface language",
                              new ComboBoxFactory("English", QStringList() << "English" << "Russian"),
                              true, "Main");
    items << new SettingsItem("Autostart", "2", "Run on system start",
                              new CheckBoxFactory(true), true, "Main");
    items << new SettingsItem("Timeout", "3", "Request timeout",
                              new SpinBoxFactory(300, 100, 10000), true, "Main");
    items << new SettingsItem("Template", "4", "Template of files searching",
                              new LineEditFactory("*.png"), true, "Template");
    items << new SettingsItem("Storage", "5", "Storage file path",
                              new FileBrowseFactory(new LineEditFactory("D:/storage"),
                                                    new PushButtonFactory("Browse...")),
                              true, "Template");

    // Организация элементов по группам (вкладкам)
    QMap<QString, QWidget*> tabMap;
    QMap<QString, QVBoxLayout*> layoutMap;

    for (SettingsItem* item : std::as_const(items)) {
        QString groupName = item->groupName();
        if (!tabMap.contains(groupName)) {
            QWidget* tab = new QWidget();
            QVBoxLayout* layout = new QVBoxLayout(tab);
            tabMap[groupName] = tab;
            layoutMap[groupName] = layout;
            tabWidget->addTab(tab, groupName);
        }

        layoutMap[groupName]->addLayout(item->createWidget());
    }

    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);

    loadSettings(); // Загрузка настроек
    connectSignalsForAutoSave(); // Подключение сигналов для автосохранения(ну по факту сохранение при изменении настроек)

    setWindowTitle("Settings Window");
}

SettingsWindow::~SettingsWindow() {
    qDeleteAll(items);
}

void SettingsWindow::loadSettings() {
    QSettings settings("TestLabs", "TestSettings");

    for (SettingsItem* item : std::as_const(items)) {
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
            } else if (QWidget* fileBrowse = item->controlWidget()) {
                QLineEdit* lineEdit = fileBrowse->findChild<QLineEdit*>();
                if (lineEdit) {
                    lineEdit->setText(value.toString());
                }
            }
        }
        settings.endGroup();
    }
}

void SettingsWindow::saveSettings() {
    QSettings settings("TestLabs", "TestSettings");

    for (SettingsItem* item : std::as_const(items)) {
        if (!item->isSavingEnabled()) continue;

        settings.beginGroup(item->groupName());
        settings.setValue(item->id(), item->getValue());
        settings.endGroup();
    }
}

void SettingsWindow::connectSignalsForAutoSave() {
    for (SettingsItem* item : std::as_const(items)) {
        if (!item->isSavingEnabled()) continue;

        if (QComboBox* comboBox = item->comboBox()) {
            connect(comboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
                    [this]() { saveSettings(); });
        } else if (QCheckBox* checkBox = item->checkBox()) {
            connect(checkBox, &QCheckBox::toggled, [this]() { saveSettings(); });
        } else if (QSpinBox* spinBox = item->spinBox()) {
            connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this]() { saveSettings(); });
        } else if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(item->controlWidget())) {
            connect(lineEdit, &QLineEdit::textChanged, [this]() { saveSettings(); });
        } else if (QWidget* fileBrowse = item->controlWidget()) {
            QLineEdit* lineEdit = fileBrowse->findChild<QLineEdit*>();
            if (lineEdit) {
                connect(lineEdit, &QLineEdit::textChanged, [this]() { saveSettings(); });
            }
        }
    }
}
