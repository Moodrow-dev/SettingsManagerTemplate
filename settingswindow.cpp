// settingswindow.cpp
#include "settingswindow.h"
#include "settingsitem.h"
#include "spinboxfactory.h"
#include "comboboxfactory.h"
#include "checkboxfactory.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QHBoxLayout>

SettingsWindow::SettingsWindow(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Пример списка настроек
    QList<SettingsItem*> items;
    items << new SettingsItem("Language", "1", "English", "Select interface language",
                              new ComboBoxFactory(QStringList() << "English" << "Russian"));
    items << new SettingsItem("Autostart", "2", true, "Run on system start",
                              new CheckBoxFactory());
    items << new SettingsItem("Timeout", "3", 300, "Request timeout",
                              new SpinBoxFactory());

    // Динамическое создание виджетов
    for (SettingsItem* item : std::as_const(items)) {
        // Создаем горизонтальный макет для каждой строки
        QHBoxLayout* rowLayout = new QHBoxLayout();

        // Создаем метку с именем
        QLabel* nameLabel = new QLabel(item->name());

        // Создаем метку для подсказки с серым текстом
        QLabel* hintLabel = new QLabel(item->description());
        hintLabel->setStyleSheet("color: gray;");

        // Объединяем имя и подсказку в вертикальный макет (слева)
        QVBoxLayout* leftLayout = new QVBoxLayout();
        leftLayout->addWidget(nameLabel);
        leftLayout->addWidget(hintLabel);

        // Создаем виджет управления
        QWidget* control = item->factory()->create();

        // Добавляем левую часть (имя + подсказка) и правую часть (виджет) в горизонтальный макет
        rowLayout->addLayout(leftLayout);
        rowLayout->addWidget(control, 1); // Виджет растягивается, если нужно

        // Добавляем горизонтальный макет в основной QFormLayout (или прямо в QVBoxLayout)
        mainLayout->addLayout(rowLayout);
    }

    setLayout(mainLayout);
}
