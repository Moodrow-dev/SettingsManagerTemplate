#include "settingswindow.h"
#include "settingsitem.h"
#include "comboboxfactory.h"
#include "spinboxfactory.h"
#include "checkboxfactory.h"
#include "lineeditfactory.h"
#include "pushbuttonfactory.h"
#include "filebrowsefactory.h"
#include "colordialogfactory.h"
#include "settingswidgetbuilder.h"
#include <QVBoxLayout>
#include <QCheckBox>


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
    items << new SettingsItem("Color", "6", "Just a color widget", new ColorDialogFactory(new LineEditFactory("#00000000"), new PushButtonFactory("Choose")),true,"Color");

    SettingsWidgetBuilder* widgetBuilder = new SettingsWidgetBuilder(items);


    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(widgetBuilder->getEmbeddedWidget());

    setLayout(mainLayout);

    setWindowTitle("Settings Window");
}

SettingsWindow::~SettingsWindow() {
    qDeleteAll(items);
}
