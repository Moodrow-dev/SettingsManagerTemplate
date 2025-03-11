#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include "settingsitem.h"
#include <QList>
#include <QMap>

class SettingsWindow : public QWidget {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private:
    void loadSettings();
    void saveSettings();
    void connectSignalsForAutoSave();

    QList<SettingsItem*> items; // Список всех элементов
};

#endif // SETTINGSWINDOW_H
