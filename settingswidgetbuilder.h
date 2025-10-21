#ifndef SETTINGSWIDGETBUILDER_H
#define SETTINGSWIDGETBUILDER_H

#include <QTreeWidgetItem>

#include "settingsitem.h"

class QScrollArea;
class QTreeWidget;
class QStackedWidget;

class SettingsWidgetBuilder {
public:
    static QWidget* buildSettingsWidget(SettingsItem* rootItem);

private:
    using PageMap = QHash<SettingsItem*, QScrollArea*>;

    static void populateTree(QTreeWidget* tree, SettingsItem* item,
                            QTreeWidgetItem* parent, PageMap& pageMap);
    static QScrollArea* createGroupPage(SettingsItem* group);
    static void connectTreeToStack(QTreeWidget* tree, QStackedWidget* stack, const PageMap& pageMap);
    static void connectAutoSave(SettingsItem* root);
    static void loadSettings(SettingsItem* root);
    static void applyValue(SettingsItem* item, const QVariant& value);
};

#endif