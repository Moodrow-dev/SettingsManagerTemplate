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

SettingsWindow::SettingsWindow(QWidget* parent) : QWidget(parent) {

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QList<SettingsItem*> items;
    items << new SettingsItem("Language", "1", "Select interface language",
                              new ComboBoxFactory("English", QStringList() << "English" << "Russian"));
    items << new SettingsItem("Autostart", "2", "Run on system start",
                              new CheckBoxFactory(true));
    items << new SettingsItem("Timeout", "3", "Request timeout",
                              new SpinBoxFactory(300, 100, 10000));
    items << new SettingsItem("Template", "4", "Template of files searching",
                              new LineEditFactory("*.png"));
    items << new SettingsItem("Storage", "5", "Storage file path",
                              new FileBrowseFactory(new LineEditFactory("D:/storage"),
                                                    new PushButtonFactory("Browse...")));

    for (SettingsItem* item : std::as_const(items)) {
        mainLayout->addLayout(item->createWidget());
    }

    mainLayout->addStretch();

    setLayout(mainLayout);

    setWindowTitle("Settings Window");
}
