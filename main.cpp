#include <QApplication>
#include "settingswindow.h"
#include "Settings.h"

#include "checkboxfactory.h"
#include "colordialogfactory.h"
#include "comboboxfactory.h"
#include "filebrowsefactory.h"
#include "lineeditfactory.h"
#include "settingsitem.h"
#include "settingswidgetbuilder.h"
#include "spinboxfactory.h"

SettingsItem* settingsWindowStructure() {
    SettingsItem *rootItem = new SettingsItem(nullptr, "Settings", "root", "Application Settings");

    SettingsItem* mainGroup = new SettingsItem(rootItem, "Main Settings", "main_group", "General application settings");
    SettingsItem* templateGroup = new SettingsItem(rootItem, "Template Settings", "template_group", "File template settings");
    SettingsItem* colorGroup = new SettingsItem(rootItem, "Appearance", "appearance_group", "Visual appearance settings");

    SettingsItem* languageItem = new SettingsItem(mainGroup, "Language", "1", "Select interface language",
                                                  new ComboBoxFactory("English", QStringList() << "English" << "Russian" << "Spanish"), true);

    SettingsItem* autostartItem = new SettingsItem(mainGroup, "Autostart", "2", "Run application on system startup",
                                                   new CheckBoxFactory(true), true);

    SettingsItem* timeoutItem = new SettingsItem(mainGroup, "Timeout", "3", "Request timeout in milliseconds",
                                                 new SpinBoxFactory(300, 100, 10000), true);

    // Добавляем настройки в группу Template
    SettingsItem* fileTemplateItem = new SettingsItem(templateGroup, "File Template", "4", "Template for file searching",
                                                      new LineEditFactory("*.png"), true);

    SettingsItem* storageItem = new SettingsItem(templateGroup, "Storage Path", "5", "Location where files will be stored",
                                                 new FileBrowseFactory(new LineEditFactory("D:/storage"), new PushButtonFactory("Browse...")), true);

    // Добавляем настройки в группу Appearance
    SettingsItem* themeItem = new SettingsItem(colorGroup, "Theme Color", "6", "Choose application theme color",
                                               new ColorDialogFactory(new LineEditFactory("#0078d4"), new PushButtonFactory("Choose Color")), true);

    SettingsItem* fontSizeItem = new SettingsItem(colorGroup, "Font Size", "7", "Application font size",
                                                  new SpinBoxFactory(12, 8, 24), true);

    return rootItem;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // СОЗДАЕМ ОБЪЕКТ НАСТРОЕК
    Settings appSettings;

    // МЕНЯЕМ ЗНАЧЕНИЯ
    appSettings.setLanguage("Russian");
    appSettings.setAutostart(false);
    appSettings.setTimeout(500);
    appSettings.setFileTemplate("*.jpg;*.png");
    appSettings.setStoragePath("E:/MyFiles");
    appSettings.setThemeColor("#ff0000");
    appSettings.setFontSize(14);

    // ВЫВОДИМ В КОНСОЛЬ
    appSettings.printSettings();

    // 1. СТРУКТУРА
    SettingsItem* structure = settingsWindowStructure();

    // 2. СОЗДАЕМ КАРТУ ДЛЯ ВИДЖЕТОВ
    QMap<QString, QWidget*> itemWidgets;

    // 3. БИБЛИОТЕКА СТРОИТ ВИДЖЕТ - ТЕПЕРЬ С КАРТОЙ!
    QWidget* settingsWidget = SettingsWidgetBuilder::buildSettingsWidget(structure, itemWidgets,&appSettings);

    // 4. ПРОСТОЕ ОКНО
    SettingsWindow* window = new SettingsWindow(settingsWidget);
    window->show();

    return app.exec();
};