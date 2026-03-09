#include "settingswidgetbuilder.h"
#include "settingsitem.h"
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QScrollArea>
#include <QLabel>
#include <QDebug>
#include <QFrame>
#include <QGroupBox>

#include "SettingsAccess.h"

void SettingsWidgetBuilder::populateTree(QTreeWidget* tree,
                                         SettingsAccess* settings,
                                         SettingsItem* item,
                                         QTreeWidgetItem* parent,
                                         PageMap& pageMap,
                                         QMap<QString, QWidget*>& itemWidgets) {  // Новый параметр
    QTreeWidgetItem* treeItem = new QTreeWidgetItem(parent);
    treeItem->setText(0, item->name());
    treeItem->setData(0, Qt::UserRole, QVariant::fromValue(item));

    if (item->isGroup()) {
        QFont font = treeItem->font(0);
        font.setBold(true);
        treeItem->setFont(0, font);

        // СОЗДАЕМ СТРАНИЦУ ДЛЯ ГРУППЫ
        QScrollArea* page = createGroupPage(settings, item, itemWidgets);  // Передаем itemWidgets дальше
        pageMap[item] = page;
    }

    if (!parent) {
        tree->addTopLevelItem(treeItem);
    }

    for (int i = 0; i < item->childCount(); ++i) {
        populateTree(tree, settings, item->child(i), treeItem, pageMap, itemWidgets);  // Передаем дальше
    }
}

void SettingsWidgetBuilder::connectTreeToStack(QTreeWidget* tree, QStackedWidget* stack, const PageMap& pageMap) {
    QObject::connect(tree, &QTreeWidget::currentItemChanged,
                     [stack, pageMap](QTreeWidgetItem* current) {
                         if (!current) return;

                         SettingsItem* item = current->data(0, Qt::UserRole).value<SettingsItem*>();
                         if (!item || !item->isGroup()) {
                             if (stack->count() > 0) {
                                 stack->setCurrentIndex(0);
                             }
                             return;
                         }

                         if (pageMap.contains(item)) {
                             QScrollArea* page = pageMap.value(item);
                             stack->setCurrentWidget(page);
                             qDebug() << "Switched to group:" << item->name();
                         }
                     });
}

QWidget* SettingsWidgetBuilder::buildSettingsWidget(SettingsItem* rootItem, SettingsAccess* appSettings) {
    qDebug() << "🔨 Building COMPLETE settings widget...";

    // 1. КОНТЕЙНЕР
    QWidget* container = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 2. ДЕРЕВО
    QTreeWidget* tree = new QTreeWidget();
    tree->setFixedWidth(250);
    tree->setHeaderHidden(true);

    // 3. ПАНЕЛЬ КОНТЕНТА
    QStackedWidget* stack = new QStackedWidget();

    // 4. СОЗДАЕМ СТРУКТУРУ ДАННЫХ
    PageMap pageMap;
    QMap<QString, QWidget*> itemWidgets;
    populateTree(tree, appSettings, rootItem, nullptr, pageMap, itemWidgets); // ← Добавили itemWidgets

    // 5. ПРЯМОЙ ЦИКЛ ПО KEYS!
    for (SettingsItem* item : pageMap.keys()) {
        QScrollArea* page = pageMap[item];
        stack->addWidget(page);
        qDebug() << "Added page for:" << item->name();
    }

    // 6. СОБИРАЕМ
    mainLayout->addWidget(tree);
    mainLayout->addWidget(stack, 1);

    // 7. ПОДКЛЮЧЕНИЯ
    connectTreeToStack(tree, stack, pageMap);

    // 8. АВТОЗАГРУЗКА - ТЕПЕРЬ ТУТ ПЕРЕДАЕМ КАРТУ!
    loadSettings(rootItem, itemWidgets, appSettings);

    tree->expandAll();
    if (tree->topLevelItemCount() > 0) {
        tree->setCurrentItem(tree->topLevelItem(0));
    }

    qDebug() << "✅ Settings widget BUILT with" << pageMap.size() << "pages!";
    return container;
}

QScrollArea* SettingsWidgetBuilder::createGroupPage(SettingsAccess* settings, SettingsItem* group,
                                                     QMap<QString, QWidget*>& itemWidgets) {  // Новый параметр
    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    QWidget* content = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(content);
    layout->setSpacing(15);
    layout->setContentsMargins(25, 25, 25, 25);

    // Заголовок
    QLabel* title = new QLabel(group->name());
    QFont font = title->font();
    font.setPointSize(16);
    font.setBold(true);
    title->setFont(font);
    layout->addWidget(title);

    if (!group->description().isEmpty()) {
        QLabel* desc = new QLabel(group->description());
        desc->setWordWrap(true);
        layout->addWidget(desc);
    }

    // Настройки
    bool hasSettings = false;
    for (int i = 0; i < group->childCount(); ++i) {
        SettingsItem* child = group->child(i);
        if (!child->isGroup()) {
          if (QHBoxLayout* widgetLayout = createSettingsItemLayout(settings, child)) {
                if (hasSettings) {
                    QFrame* sep = new QFrame();
                    sep->setFrameShape(QFrame::HLine);
                    sep->setFrameShadow(QFrame::Sunken);
                    layout->addWidget(sep);
                }

                // СОХРАНЯЕМ ВИДЖЕТ В КАРТУ!
                if (widgetLayout->count() > 1) {
                    QWidget* controlWidget = widgetLayout->itemAt(1)->widget();
                    if (controlWidget) {
                        itemWidgets[child->id()] = controlWidget;
                        qDebug() << "💾 Saved widget for" << child->name() << "with ID" << child->id();
                    }
                }

                layout->addLayout(widgetLayout);
                hasSettings = true;
            }
        }
    }

    if (!hasSettings) {
        QLabel* empty = new QLabel("Нет доступных настроек");
        empty->setAlignment(Qt::AlignCenter);
        layout->addWidget(empty);
    }

    layout->addStretch();
    scroll->setWidget(content);
    return scroll;
}

void SettingsWidgetBuilder::loadSettings(SettingsItem* root, const QMap<QString, QWidget*>& itemWidgets, SettingsAccess* appSettings) {
    qDebug() << "📂 Loading settings from appSettings...";

    QList<SettingsItem*> all = root->getAllChildren();
    all.prepend(root);

    int count = 0;
    for (SettingsItem* item : all) {
        if (!item->isSavingEnabled() || item->isGroup()) continue;

        const QString itemId = item->id();
        QVariant value = appSettings->getValue(*item);

        QWidget* widget = itemWidgets.value(itemId);
        if (widget && value.isValid()) {
            SettingsControlFactory* factory = item->factory();
            if (factory)
                factory->setValueToWidget(widget, value);
            count++;
            qDebug() << "   ✅ Loaded" << item->name() << "=" << value;
        }
    }
    qDebug() << "📂 Loaded" << count << "settings from appSettings";
}

QWidget* SettingsWidgetBuilder::createSettingsRow(SettingsAccess* settings, SettingsItem* item,
                                                  QMap<QString, QWidget*>& itemWidgets) {
    QHBoxLayout* layout = createSettingsItemLayout(settings, item);
    if (!layout) return nullptr;

    QWidget* container = new QWidget();
    container->setLayout(layout);

    // Сохраняем контрольный виджет в карту по ID
    if (layout->count() > 1) {
        QWidget* controlWidget = layout->itemAt(1)->widget();
        if (controlWidget) {
            itemWidgets[item->id()] = controlWidget;
            qDebug() << "Saved widget for" << item->name() << "with ID" << item->id();
        }
    }

    return container;
}

QHBoxLayout* SettingsWidgetBuilder::createSettingsItemLayout(SettingsAccess* settings, SettingsItem* item)
{
    if (item->isGroup()) {
        return nullptr;
    }

    SettingsControlFactory* factory = item->factory();
    if (!factory)
        return nullptr;

    QHBoxLayout* rowLayout = new QHBoxLayout();
    QVBoxLayout* leftLayout = new QVBoxLayout();
    QLabel* nameLabel = new QLabel(item->name());
    QLabel* hintLabel = new QLabel(item->description());
    leftLayout->addWidget(nameLabel);
    leftLayout->addWidget(hintLabel);

    auto callback = [settings](SettingsItem* settingsItem, const QVariant& value) {
        settings->setValue(*settingsItem, value);
    };
    // Создаем виджет
    QWidget* controlWidget = nullptr;
    if (item->isSavingEnabled())
        controlWidget = factory->create(item, callback);
    else
        controlWidget = factory->create(item, nullptr);
    factory->setValueToWidget(controlWidget, item->defaultValue());

    rowLayout->addLayout(leftLayout);
    rowLayout->addWidget(controlWidget, 1);

    return rowLayout;
}
