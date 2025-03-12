#ifndef SETTINGSWIDGETBUILDER_H
#define SETTINGSWIDGETBUILDER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QScrollArea>
#include "settingsitem.h"

class SettingsWidgetBuilder : public QObject {
    Q_OBJECT

public:
    explicit SettingsWidgetBuilder(QList<SettingsItem*> widgetList, QObject* parent = nullptr);
    ~SettingsWidgetBuilder();

    QWidget* getEmbeddedWidget() const;

private:
    QList<SettingsItem*> widgetList_;
    QHBoxLayout* embedLayout_; // Основной макет

    void loadSettings();
    void saveSettings();
    void connectSignalsForAutoSave();
    QScrollArea* createScrollAreaForGroup(const QString& group, const QList<SettingsItem*>& widgetList);
};

#endif // SETTINGSWIDGETBUILDER_H
