#ifndef MY1STPROJ_SETTINGS_H
#define MY1STPROJ_SETTINGS_H

#include <QString>
#include <QSettings>

#include "SettingsAccess.h"

class Settings : public SettingsAccess
{
public:
    explicit Settings(const QString& organization, const QString& application = QString(), QObject *parent = nullptr);

    void setValue(const SettingsItem& item, const QVariant& value) final;
    QVariant getValue(const SettingsItem& item) const final;

private:
    QSettings m_settings;
};

#endif //MY1STPROJ_SETTINGS_H
