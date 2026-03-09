#ifndef SETTINGSWIDGETBUILDER_H
#define SETTINGSWIDGETBUILDER_H

#include <QTreeWidgetItem>
#include <QMap>
#include <QHash>

#include "settingsitem.h"

class SettingsAccess;
class QScrollArea;
class QTreeWidget;
class QStackedWidget;

class SettingsWidgetBuilder {
public:
    // Публичные статические методы - то, что используем из main
  static QWidget* buildSettingsWidget(SettingsItem* rootItem, SettingsAccess* appSettings);
    static void loadSettings(SettingsItem* root, const QMap<QString, QWidget*>& itemWidgets);

    static void loadSettings(SettingsItem *root, const QMap<QString, QWidget *> &itemWidgets, SettingsAccess* appSettings);

private:
    using PageMap = QHash<SettingsItem*, QScrollArea*>;

    // Приватные статические методы - только для внутреннего использования
    static void populateTree(QTreeWidget* tree, SettingsAccess* settings, SettingsItem* item,
                             QTreeWidgetItem* parent, PageMap& pageMap,
                             QMap<QString, QWidget*>& itemWidgets);

    static QScrollArea* createGroupPage(SettingsAccess* settings, SettingsItem* group, QMap<QString, QWidget*>& itemWidgets);

    static void connectTreeToStack(QTreeWidget* tree, QStackedWidget* stack, const PageMap& pageMap);

    static QWidget* createSettingsRow(SettingsAccess* settings, SettingsItem* item, QMap<QString, QWidget*>& itemWidgets);

    static QHBoxLayout* createSettingsItemLayout(SettingsAccess* settings, SettingsItem* item);
};

#endif // SETTINGSWIDGETBUILDER_H
