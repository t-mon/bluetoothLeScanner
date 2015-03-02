#ifndef SERVICEINFO_H
#define SERVICEINFO_H

#include <QObject>
#include <QLowEnergyService>

class ServiceInfo : public QObject
{
    Q_OBJECT
public:
    ServiceInfo();
    ServiceInfo(QLowEnergyService *service);
    QLowEnergyService *service() const;
    QString uuidHex() const;
    QBluetoothUuid uuid() const;
    QString name() const;
    QString type() const;

signals:
    void serviceChanged();

private:
    QLowEnergyService *m_service;
};

#endif // SERVICEINFO_H
