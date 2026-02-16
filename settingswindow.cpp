#include "settingswindow.h"

SettingsWindow::SettingsWindow(QWidget* settingsWidget) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(settingsWidget);

    setWindowTitle("Настройки");
    setMinimumSize(700, 500);
}