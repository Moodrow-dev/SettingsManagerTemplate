#ifndef SETTINGSWIDGETBUILDER_H
#define SETTINGSWIDGETBUILDER_H

#include <QTreeWidgetItem>
#include <QMap>
#include <QHash>

#include "Settings.h"
#include "settingsitem.h"

class QScrollArea;
class QTreeWidget;
class QStackedWidget;

class SettingsWidgetBuilder {
public:
    // Публичные статические методы - то, что используем из main
    static QWidget* buildSettingsWidget(SettingsItem* rootItem, QMap<QString, QWidget*>& outWidgets,Settings* appSettings);
    static void loadSettings(SettingsItem* root, const QMap<QString, QWidget*>& itemWidgets);
    static void connectAutoSave(SettingsItem* root, const QMap<QString, QWidget*>& widgets);

    static void loadSettings(SettingsItem *root, const QMap<QString, QWidget *> &itemWidgets, Settings *appSettings);

private:
    using PageMap = QHash<SettingsItem*, QScrollArea*>;

    // Приватные статические методы - только для внутреннего использования
    static void populateTree(QTreeWidget* tree, SettingsItem* item,
                             QTreeWidgetItem* parent, PageMap& pageMap,
                             QMap<QString, QWidget*>& itemWidgets);

    static QScrollArea* createGroupPage(SettingsItem* group, QMap<QString, QWidget*>& itemWidgets);

    QWidget *buildItem(SettingsItem *item, QMap<QString, QWidget *> &outWidgets);

    static void connectTreeToStack(QTreeWidget* tree, QStackedWidget* stack, const PageMap& pageMap);

    static void applyValue(SettingsItem* item, QWidget* widget, const QVariant& value);

    static QWidget* createSettingsRow(SettingsItem* item, QMap<QString, QWidget*>& itemWidgets);
};

#endif // SETTINGSWIDGETBUILDER_H