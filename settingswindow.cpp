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
    items << new SettingsItem("Language", "1", "Select interface language",
                              new ComboBoxFactory("English",QStringList() << "English" << "Russian"));
    items << new SettingsItem("Autostart", "2", "Run on system start",
                              new CheckBoxFactory(true));
    items << new SettingsItem("Timeout", "3", "Request timeout",
                              new SpinBoxFactory(300,100,10000));

    // Добавляем макеты от каждого SettingsItem
    for (SettingsItem* item : std::as_const(items)) {
        mainLayout->addLayout(item->createWidget());
    }

    setLayout(mainLayout);
}
