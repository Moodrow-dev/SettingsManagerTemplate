#ifndef MY1STPROJ_SETTINGS_H
#define MY1STPROJ_SETTINGS_H
#include <QString>
#include <QMap>


class Settings {
public:
    Settings() {
        loadFromStorage();
    }

    void loadFromStorage();

    void saveToStorage();

    QString getLanguage() const;

    bool getAutostart() const;

    int getTimeout() const;

    QString getFileTemplate() const;

    QString getStoragePath() const;

    QString getThemeColor() const;

    int getFontSize() const;

    void setLanguage(const QString &lang);

    void setAutostart(bool autoStart);

    void setTimeout(int timeout);

    void setFileTemplate(const QString &templ);

    void setStoragePath(const QString &path);

    void setThemeColor(const QString &color);

    void setFontSize(int size);

    void printSettings();

private:
    QMap<QString, QVariant> m_values; // Хранилище всех настроек
};


#endif //MY1STPROJ_SETTINGS_H
