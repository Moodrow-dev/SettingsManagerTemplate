// settingswindow.cpp
#include "settingswindow.h"
#include "settingsitem.h"
#include "comboboxfactory.h"
#include "spinboxfactory.h"
#include "checkboxfactory.h"
#include <QVBoxLayout>

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

    // Добавляем макеты от каждого SettingsItem
    for (SettingsItem* item : std::as_const(items)) {
        mainLayout->addLayout(item->createWidget());
    }

    setLayout(mainLayout);
}
