#pragma once

#include <QObject>

class SettingsItem;
class QVariant;

class SettingsAccess : public QObject
{
  Q_OBJECT

public:
  virtual void setValue(const SettingsItem& item, const QVariant& value) = 0;
  virtual QVariant getValue(const SettingsItem& item) const = 0;

signals:
  void changed(const QString& itemUid, const QVariant& value);
};
