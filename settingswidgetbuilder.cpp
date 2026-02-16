#include "settingswidgetbuilder.h"
#include "settingsitem.h"
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QScrollArea>
#include <QLabel>
#include <QSettings>
#include <QDebug>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QFrame>
#include <QGroupBox>

#include "Settings.h"

void SettingsWidgetBuilder::populateTree(QTreeWidget* tree,
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
        QScrollArea* page = createGroupPage(item, itemWidgets);  // Передаем itemWidgets дальше
        pageMap[item] = page;
    }

    if (!parent) {
        tree->addTopLevelItem(treeItem);
    }

    for (int i = 0; i < item->childCount(); ++i) {
        populateTree(tree, item->child(i), treeItem, pageMap, itemWidgets);  // Передаем дальше
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

QWidget* SettingsWidgetBuilder::buildSettingsWidget(SettingsItem* rootItem, QMap<QString, QWidget*>& itemWidgets, Settings* appSettings) {
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
    populateTree(tree, rootItem, nullptr, pageMap, itemWidgets); // ← Добавили itemWidgets

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
    connectAutoSave(rootItem, itemWidgets); // ← Добавили itemWidgets

    // 8. АВТОЗАГРУЗКА - ТЕПЕРЬ ТУТ ПЕРЕДАЕМ КАРТУ!
    loadSettings(rootItem, itemWidgets, appSettings);

    tree->expandAll();
    if (tree->topLevelItemCount() > 0) {
        tree->setCurrentItem(tree->topLevelItem(0));
    }

    qDebug() << "✅ Settings widget BUILT with" << pageMap.size() << "pages!";
    return container;
}

QScrollArea* SettingsWidgetBuilder::createGroupPage(SettingsItem* group,
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
            if (QHBoxLayout* widgetLayout = child->createWidget()) {
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

QWidget* SettingsWidgetBuilder::buildItem(SettingsItem* item, QMap<QString, QWidget*>& outWidgets) {
    QHBoxLayout* layout = item->createWidget();
    if (!layout) return nullptr;

    QWidget* container = new QWidget();
    container->setLayout(layout);

    // Сохраняем виджет в карту по ID
    if (layout->count() > 1) {
        QWidget* controlWidget = layout->itemAt(1)->widget();
        if (controlWidget) {
            outWidgets[item->id()] = controlWidget;
        }
    }

    return container;
}

void SettingsWidgetBuilder::connectAutoSave(SettingsItem* root, const QMap<QString, QWidget*>& widgets) {
    qDebug() << "🔌 Connecting auto-save...";

    QList<SettingsItem*> all = root->getAllChildren();
    all.prepend(root);

    int connected = 0;
    for (SettingsItem* item : all) {
        if (!item->isSavingEnabled() || item->isGroup()) continue;

        QWidget* widget = widgets.value(item->id());
        if (!widget) {
            qDebug() << "⚠️ No widget for" << item->name() << "ID:" << item->id();
            continue;
        }

        qDebug() << "🔌 Connecting" << item->name() << "ID:" << item->id();

        auto saveThisSetting = [item, widget]() {
            QVariant value;
            if (auto* combo = qobject_cast<QComboBox*>(widget)) {
                value = combo->currentText();
                qDebug() << "💾 SAVING ComboBox" << item->name() << "=" << value;
            } else if (auto* check = qobject_cast<QCheckBox*>(widget)) {
                value = check->isChecked();
                qDebug() << "💾 SAVING CheckBox" << item->name() << "=" << value;
            } else if (auto* spin = qobject_cast<QSpinBox*>(widget)) {
                value = spin->value();
                qDebug() << "💾 SAVING SpinBox" << item->name() << "=" << value;
            } else if (auto* line = widget->findChild<QLineEdit*>()) {
                value = line->text();
                qDebug() << "💾 SAVING LineEdit" << item->name() << "=" << value;
            }

            QSettings settings("TestLabs", "TestSettings");
            settings.setValue(item->id(), value);
            settings.sync();

            // Проверим, сохранилось ли
            qDebug() << "   ✅ Saved to:" << settings.fileName();
            qDebug() << "   🔍 Read back:" << settings.value(item->id());
        };

        if (auto* combo = qobject_cast<QComboBox*>(widget)) {
            QObject::connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), saveThisSetting);
            connected++;
        } else if (auto* check = qobject_cast<QCheckBox*>(widget)) {
            QObject::connect(check, &QCheckBox::toggled, saveThisSetting);
            connected++;
        } else if (auto* spin = qobject_cast<QSpinBox*>(widget)) {
            QObject::connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), saveThisSetting);
            connected++;
        } else {
            if (auto* line = widget->findChild<QLineEdit*>()) {
                QObject::connect(line, &QLineEdit::textChanged, saveThisSetting);
                connected++;
            }
        }
    }
    qDebug() << "✅ Connected" << connected << "settings for auto-save";
}

void SettingsWidgetBuilder::loadSettings(SettingsItem* root, const QMap<QString, QWidget*>& itemWidgets, Settings* appSettings) {
    qDebug() << "📂 Loading settings from appSettings...";

    QList<SettingsItem*> all = root->getAllChildren();
    all.prepend(root);

    int count = 0;
    for (SettingsItem* item : all) {
        if (!item->isSavingEnabled() || item->isGroup()) continue;

        QString key = item->id();
        QVariant value;

        // Берем значение из appSettings по ID
        if (key == "1") value = appSettings->getLanguage();
        else if (key == "2") value = appSettings->getAutostart();
        else if (key == "3") value = appSettings->getTimeout();
        else if (key == "4") value = appSettings->getFileTemplate();
        else if (key == "5") value = appSettings->getStoragePath();
        else if (key == "6") value = appSettings->getThemeColor();
        else if (key == "7") value = appSettings->getFontSize();

        QWidget* widget = itemWidgets.value(key);
        if (widget && value.isValid()) {
            item->setValueToWidget(widget, value);
            count++;
            qDebug() << "   ✅ Loaded" << item->name() << "=" << value;
        }
    }
    qDebug() << "📂 Loaded" << count << "settings from appSettings";
}

void SettingsWidgetBuilder::applyValue(SettingsItem* item, QWidget* widget, const QVariant& value) {
    if (!widget) return;

    // Используем новый метод из SettingsItem
    item->setValueToWidget(widget, value);
}

QWidget* SettingsWidgetBuilder::createSettingsRow(SettingsItem* item,
                                                  QMap<QString, QWidget*>& itemWidgets) {
    QHBoxLayout* layout = item->createWidget();
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
