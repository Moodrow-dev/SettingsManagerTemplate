#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include "settingsitem.h"
#include <QList>

class SettingsWindow : public QWidget {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private:
    void loadSettings();
    void saveSettings();
    void connectSignalsForAutoSave();

    QList<SettingsItem*> mainTabItems;
    QList<SettingsItem*> templateTabItems;
};

#endif
