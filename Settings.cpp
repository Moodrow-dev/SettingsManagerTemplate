#include "Settings.h"
#include <QDebug>
#include <QSettings>
#include <QString>
#include <QVariant>

#include "settingsitem.h"

Settings::Settings(const QString& organization, const QString& application, QObject* parent) :
    m_settings(organization, application, parent)
{
  qDebug() << "Start " << m_settings.fileName();
}

void Settings::setValue(const SettingsItem& item, const QVariant& value)
{
    const QString itemId = item.id();
    const QVariant prevValue = m_settings.value(itemId);
    if (prevValue == value)
        return;

    m_settings.setValue(itemId, value);
    emit changed(itemId, value);
}

QVariant Settings::getValue(const SettingsItem& item) const
{
    return m_settings.value(item.id(), item.defaultValue());
}
