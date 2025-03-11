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
#include <QSettings>
#include <QLabel>

SettingsWindow::SettingsWindow(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QTabWidget* tabWidget = new QTabWidget(this);

    // Создание элементов для первой вкладки
    mainTabItems << new SettingsItem("Language", "1", "Select interface language",
                                  new ComboBoxFactory("English", QStringList() << "English" << "Russian"));
    mainTabItems << new SettingsItem("Autostart", "2", "Run on system start",
                                  new CheckBoxFactory(true));
    mainTabItems << new SettingsItem("Timeout", "3", "Request timeout",
                                  new SpinBoxFactory(300, 100, 10000));

    // Создание элементов для второй вкладки
    templateTabItems << new SettingsItem("Template", "4", "Template of files searching",
                                  new LineEditFactory("*.png"));
    templateTabItems << new SettingsItem("Storage", "5", "Storage file path",
                                  new FileBrowseFactory(new LineEditFactory("D:/storage"),
                                                        new PushButtonFactory("Browse...")));

    // Создание первой вкладки
    QWidget* mainTab = new QWidget();
    QVBoxLayout* mainTabLayout = new QVBoxLayout(mainTab);
    for (SettingsItem* item : std::as_const(mainTabItems)) {
        mainTabLayout->addLayout(item->createWidget());
    }

    // Создание второй вкладки
    QWidget* storageTab = new QWidget();
    QVBoxLayout* storageTabLayout = new QVBoxLayout(storageTab);
    for (SettingsItem* item : std::as_const(templateTabItems)) {
        storageTabLayout->addLayout(item->createWidget());
    }

    // Добавление вкладок в QTabWidget
    tabWidget->addTab(mainTab, "General");
    tabWidget->addTab(storageTab, "Storage");

    // Основной макет окна
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);

    // Загрузка сохранённых настроек
    loadSettings();

    // Подключение автоматического сохранения при изменении значений
    connectSignalsForAutoSave();

    setWindowTitle("Settings Window");
}

SettingsWindow::~SettingsWindow() {
    qDeleteAll(mainTabItems);
    qDeleteAll(templateTabItems);
}

void SettingsWindow::loadSettings() {
    QSettings settings("TestLabs", "TestSettings");

    settings.beginGroup("Main");
    for (SettingsItem* item : mainTabItems) {
        QVariant value = settings.value(item->id(), item->getValue());
        if (value.isValid()) {
            if (QComboBox* comboBox = item->comboBox()) {
                comboBox->setCurrentText(value.toString());
            } else if (QCheckBox* checkBox = item->checkBox()) {
                checkBox->setChecked(value.toBool());
            } else if (QSpinBox* spinBox = item->spinBox()) {
                spinBox->setValue(value.toInt());
            }
        }
    }
    settings.endGroup();

    settings.beginGroup("Template");
    for (SettingsItem* item : templateTabItems) {
        QVariant value = settings.value(item->id(), item->getValue());
        if (value.isValid()) {
            if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(item->controlWidget())) {
                lineEdit->setText(value.toString());
            } else if (QWidget* fileBrowse = item->controlWidget()) {
                QLineEdit* lineEdit = fileBrowse->findChild<QLineEdit*>();
                if (lineEdit) {
                    lineEdit->setText(value.toString());
                }
            }
        }
    }
    settings.endGroup();
}

void SettingsWindow::saveSettings() {
    QSettings settings("TestLabs", "TestSettings");

    settings.beginGroup("Main");
    for (SettingsItem* item : mainTabItems) {
        settings.setValue(item->id(), item->getValue());
    }
    settings.endGroup();

    settings.beginGroup("Template");
    for (SettingsItem* item : templateTabItems) {
        settings.setValue(item->id(), item->getValue());
    }
    settings.endGroup();
}

void SettingsWindow::connectSignalsForAutoSave() {
    for (SettingsItem* item : mainTabItems) {
        if (QComboBox* comboBox = item->comboBox()) {
            connect(comboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
                    [this]() { saveSettings(); });
        } else if (QCheckBox* checkBox = item->checkBox()) {
            connect(checkBox, &QCheckBox::toggled, [this]() { saveSettings(); });
        } else if (QSpinBox* spinBox = item->spinBox()) {
            connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this]() { saveSettings(); });
        }
    }

    for (SettingsItem* item : templateTabItems) {
        if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(item->controlWidget())) {
            connect(lineEdit, &QLineEdit::textChanged, [this]() { saveSettings(); });
        } else if (QWidget* fileBrowse = item->controlWidget()) {
            QLineEdit* lineEdit = fileBrowse->findChild<QLineEdit*>();
            if (lineEdit) {
                connect(lineEdit, &QLineEdit::textChanged, [this]() { saveSettings(); });
            }
        }
    }
}
