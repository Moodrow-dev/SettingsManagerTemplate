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

QWidget* SettingsWidgetBuilder::buildSettingsWidget(SettingsItem* rootItem) {
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
    populateTree(tree, rootItem, nullptr, pageMap);

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
    connectAutoSave(rootItem);

    // 8. АВТОЗАГРУЗКА
    loadSettings(rootItem);

    tree->expandAll();
    if (tree->topLevelItemCount() > 0) {
        tree->setCurrentItem(tree->topLevelItem(0));
    }

    qDebug() << "✅ Settings widget BUILT with" << pageMap.size() << "pages!";
    return container;
}

void SettingsWidgetBuilder::populateTree(QTreeWidget* tree, SettingsItem* item,
                                         QTreeWidgetItem* parent, PageMap& pageMap) {
    QTreeWidgetItem* treeItem = new QTreeWidgetItem(parent);
    treeItem->setText(0, item->name());
    treeItem->setData(0, Qt::UserRole, QVariant::fromValue(item));

    if (item->isGroup()) {
        QFont font = treeItem->font(0);
        font.setBold(true);
        treeItem->setFont(0, font);

        // СОЗДАЕМ СТРАНИЦУ ДЛЯ ГРУППЫ
        QScrollArea* page = createGroupPage(item);
        pageMap[item] = page;
    }

    if (!parent) {
        tree->addTopLevelItem(treeItem);
    }

    for (int i = 0; i < item->childCount(); ++i) {
        populateTree(tree, item->child(i), treeItem, pageMap);
    }
}

QScrollArea* SettingsWidgetBuilder::createGroupPage(SettingsItem* group) {
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

void SettingsWidgetBuilder::connectAutoSave(SettingsItem* root) {
    auto saveAll = []() {
        QSettings settings("TestLabs", "TestSettings");
        settings.sync();
        qDebug() << "💾 Auto-saved!";
    };

    QList<SettingsItem*> all = root->getAllChildren();
    all.prepend(root);

    for (SettingsItem* item : all) {
        if (!item->isSavingEnabled() || item->isGroup()) continue;

        QWidget* widget = item->getControlWidget();
        if (!widget) continue;

        if (auto* combo = qobject_cast<QComboBox*>(widget)) {
            QObject::connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), saveAll);
        } else if (auto* check = qobject_cast<QCheckBox*>(widget)) {
            QObject::connect(check, &QCheckBox::toggled, saveAll);
        } else if (auto* spin = qobject_cast<QSpinBox*>(widget)) {
            QObject::connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), saveAll);
        } else {
            if (auto* line = widget->findChild<QLineEdit*>()) {
                QObject::connect(line, &QLineEdit::textChanged, saveAll);
            }
        }
    }
}

void SettingsWidgetBuilder::loadSettings(SettingsItem* root) {
    QSettings settings("TestLabs", "TestSettings");
    QList<SettingsItem*> all = root->getAllChildren();
    all.prepend(root);

    int count = 0;
    for (SettingsItem* item : all) {
        if (!item->isSavingEnabled() || item->isGroup()) continue;

        QString key = item->id();
        QVariant saved = settings.value(key, item->getValue());
        applyValue(item, saved);
        count++;
    }
    qDebug() << "Loaded" << count << "settings";
}

void SettingsWidgetBuilder::applyValue(SettingsItem* item, const QVariant& value) {
    if (QComboBox* combo = item->getComboBox()) {
        int idx = combo->findText(value.toString());
        if (idx >= 0) combo->setCurrentIndex(idx);
    } else if (QCheckBox* check = item->getCheckBox()) {
        check->setChecked(value.toBool());
    } else if (QSpinBox* spin = item->getSpinBox()) {
        spin->setValue(value.toInt());
    } else if (QWidget* w = item->getControlWidget()) {
        if (QLineEdit* line = qobject_cast<QLineEdit*>(w)) {
            line->setText(value.toString());
        } else if (QLineEdit* line = w->findChild<QLineEdit*>()) {
            line->setText(value.toString());
        }
    }
}