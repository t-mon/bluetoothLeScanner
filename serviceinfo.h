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
    QString getUuid() const;
    QString getName() const;
    QString getType() const;

signals:
    void serviceChanged();

private:
    QLowEnergyService *m_service;
};

#endif // SERVICEINFO_H
