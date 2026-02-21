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

struct AppSettingsItems
{
  static SettingsItem RootItem;

  static SettingsItem MainGroup;
  static SettingsItem TemplateGroup;
  static SettingsItem ColorGroup;

  static SettingsItem LanguageItem;
  static SettingsItem AutostartItem;
  static SettingsItem TimeoutItem;

  static SettingsItem FileTemplateItem;
  static SettingsItem StorageItem;

  static SettingsItem ThemeItem;
  static SettingsItem FontSizeItem;
};

SettingsItem AppSettingsItems::RootItem = SettingsItem("root", "Settings", "Application Settings");
SettingsItem AppSettingsItems::MainGroup = SettingsItem("main_group", "Main Settings", "General application settings", &RootItem);
SettingsItem AppSettingsItems::TemplateGroup = SettingsItem("template_group", "Template Settings", "File template settings", &RootItem);
SettingsItem AppSettingsItems::ColorGroup = SettingsItem("appearance_group", "Appearance", "Visual appearance settings", &RootItem);

SettingsItem AppSettingsItems::LanguageItem = SettingsItem("lang", "Language", "Select interface language", "English", &MainGroup,
  new ComboBoxFactory(QStringList() << "English" << "Russian" << "Spanish"), true);
SettingsItem AppSettingsItems::AutostartItem = SettingsItem("autostart", "Autostart", "Run application on system startup", true, &MainGroup,
  new CheckBoxFactory, true);
SettingsItem AppSettingsItems::TimeoutItem = SettingsItem("timeout", "Timeout", "Request timeout in milliseconds", 300, &MainGroup,
  new SpinBoxFactory(100, 10000), true);

SettingsItem AppSettingsItems::FileTemplateItem = SettingsItem("file_template", "File Template", "Template for file searching", "*.png", &TemplateGroup,
  new LineEditFactory, true);
SettingsItem AppSettingsItems::StorageItem = SettingsItem("storage", "Storage Path", "Location where files will be stored", "", &TemplateGroup,
  new FileBrowseFactory, true);

SettingsItem AppSettingsItems::ThemeItem = SettingsItem("theme", "Theme Color", "Choose application theme color", "#0078d4", &ColorGroup,
  new ColorDialogFactory, true);
SettingsItem AppSettingsItems::FontSizeItem = SettingsItem("font", "Font Size", "Application font size", 12, &ColorGroup,
  new SpinBoxFactory(8, 24), true);

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Settings appSettings("Test");

    appSettings.setValue(AppSettingsItems::LanguageItem, "Russian");
    appSettings.setValue(AppSettingsItems::AutostartItem, false);
    appSettings.setValue(AppSettingsItems::TimeoutItem,500);
    appSettings.setValue(AppSettingsItems::FileTemplateItem, "*.jpg;*.png");
    appSettings.setValue(AppSettingsItems::StorageItem, "E:/MyFiles");
    appSettings.setValue(AppSettingsItems::ThemeItem, "#ff0000");
    appSettings.setValue(AppSettingsItems::FontSizeItem, 14);

    QWidget* settingsWidget = SettingsWidgetBuilder::buildSettingsWidget(&AppSettingsItems::RootItem, &appSettings);
    settingsWidget->setWindowTitle("Настройки");
    settingsWidget->setMinimumSize(700, 500);
    settingsWidget->show();

    return app.exec();
};
