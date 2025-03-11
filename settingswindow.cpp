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
#include <QScrollArea>
#include <QStackedWidget>
#include <QListWidget>

SettingsWindow::SettingsWindow(QWidget* parent) : QWidget(parent) {

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

    QStringList groups = {};

    // Создаем боковую панель
    QListWidget* sidePanel = new QListWidget(this);

    // Создаем QStackedWidget для содержимого
    QStackedWidget* stackedWidget = new QStackedWidget(this);

    QMap<QString,QWidget*> pageMap;

// Вот этот цикл внизу нужен для того, чтобы генерация проходила независимо от кол-ва групп настроек

    for (SettingsItem* item : std::as_const(items)) {
        QString group = item->groupName(); // Исключительно для удобства

        if (!groups.contains(group)) {
            groups.append(group);
            sidePanel->addItem(group);

            // Создаем QScrollArea для группы
            QScrollArea* scrollArea = createScrollAreaForGroup(group, items); // Ну а функция тут как бы и не обязательна, но с ней код более читаемый
            stackedWidget->addWidget(scrollArea);
            pageMap[group] = scrollArea;
        }
    }


    connect(sidePanel, &QListWidget::currentRowChanged, stackedWidget, &QStackedWidget::setCurrentIndex);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(sidePanel, 1); // Боковая панель занимает 1 часть пространства
    mainLayout->addWidget(stackedWidget, 3); // Основное содержимое занимает 3 части
    setLayout(mainLayout);

    loadSettings(); // Загрузка настроек
    connectSignalsForAutoSave(); // Подключение сигналов для автосохранения(ну по факту сохранение при изменении настроек)

    setWindowTitle("Settings Window");
}

SettingsWindow::~SettingsWindow() {
    qDeleteAll(items);
}

QScrollArea* SettingsWindow::createScrollAreaForGroup(const QString& group, const QList<SettingsItem*>& items) {
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);

    // Создаем контейнер для содержимого
    QWidget* contentWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(contentWidget);

    // Добавляем элементы управления для группы
    for (SettingsItem* item : std::as_const(items)) {
        if (item->groupName() == group) {
            layout->addLayout(item->createWidget());
        }
    }

    scrollArea->setWidget(contentWidget);
    return scrollArea;
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
