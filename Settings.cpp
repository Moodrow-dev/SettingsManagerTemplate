#include "Settings.h"
#include <QDebug>
#include <QSettings>
#include <QString>
#include <QVariant>

QString Settings::getLanguage() const { 
    return m_values.value("1", "English").toString(); 
}

bool Settings::getAutostart() const { 
    return m_values.value("2", true).toBool(); 
}

int Settings::getTimeout() const { 
    return m_values.value("3", 300).toInt(); 
}

QString Settings::getFileTemplate() const { 
    return m_values.value("4", "*.png").toString(); 
}

QString Settings::getStoragePath() const { 
    return m_values.value("5", "D:/storage").toString(); 
}

QString Settings::getThemeColor() const { 
    return m_values.value("6", "#0078d4").toString(); 
}

int Settings::getFontSize() const { 
    return m_values.value("7", 12).toInt(); 
}

void Settings::setLanguage(const QString &lang) { 
    m_values["1"] = lang;
}

void Settings::setAutostart(bool autoStart) { 
    m_values["2"] = autoStart;
}

void Settings::setTimeout(int timeout) { 
    m_values["3"] = timeout;
}

void Settings::setFileTemplate(const QString &templ) { 
    m_values["4"] = templ;
}

void Settings::setStoragePath(const QString &path) {
    m_values["5"] = path;
}

void Settings::setThemeColor(const QString &color) { 
    m_values["6"] = color;
}

void Settings::setFontSize(int size) { 
    m_values["7"] = size;
}

void Settings::printSettings() {
    qDebug() << "=== ТЕКУЩИЕ НАСТРОЙКИ ===";
    qDebug() << "Language:" << getLanguage();
    qDebug() << "Autostart:" << (getAutostart() ? "Yes" : "No");
    qDebug() << "Timeout:" << getTimeout();
    qDebug() << "File Template:" << getFileTemplate();
    qDebug() << "Storage Path:" << getStoragePath();
    qDebug() << "Theme Color:" << getThemeColor();
    qDebug() << "Font Size:" << getFontSize();
    qDebug() << "==========================";
}

// Загрузка из постоянного хранилища
void Settings::loadFromStorage() {
    QSettings settings("TestLabs", "TestSettings");

    // Загружаем каждую настройку, если она есть в хранилище
    if (settings.contains("1")) m_values["1"] = settings.value("1");
    if (settings.contains("2")) m_values["2"] = settings.value("2");
    if (settings.contains("3")) m_values["3"] = settings.value("3");
    if (settings.contains("4")) m_values["4"] = settings.value("4");
    if (settings.contains("5")) m_values["5"] = settings.value("5");
    if (settings.contains("6")) m_values["6"] = settings.value("6");
    if (settings.contains("7")) m_values["7"] = settings.value("7");

    qDebug() << "📂 Settings loaded from storage";
}

// Сохранение в постоянное хранилище
void Settings::saveToStorage() {
    QSettings settings("TestLabs", "TestSettings");

    // Сохраняем каждую настройку
    settings.setValue("1", m_values["1"]);
    settings.setValue("2", m_values["2"]);
    settings.setValue("3", m_values["3"]);
    settings.setValue("4", m_values["4"]);
    settings.setValue("5", m_values["5"]);
    settings.setValue("6", m_values["6"]);
    settings.setValue("7", m_values["7"]);

    settings.sync();
    qDebug() << "💾 Settings saved to storage";
}