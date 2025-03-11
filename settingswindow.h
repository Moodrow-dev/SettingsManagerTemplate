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
    void connectSignalsForAutoSave(); // Новое объявление

    QList<SettingsItem*> mainTabItems; // Список элементов для первой вкладки
    QList<SettingsItem*> templateTabItems; // Список элементов для второй вкладки
};

#endif // SETTINGSWINDOW_H
