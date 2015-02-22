#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QDebug>
#include <QBluetoothHostInfo>
#include <QBluetoothDeviceInfo>
#include <QBluetoothAddress>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothServiceInfo>
#include <QLowEnergyController>
#include "characteristicinfo.h"

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(const QBluetoothDeviceInfo &deviceInfo, QObject *parent = 0);

    QString name() const;
    QBluetoothAddress address() const;
    bool isConnected() const;

private:
    QBluetoothDeviceInfo m_deviceInfo;
    QLowEnergyController *m_controller;

    QList<QBluetoothUuid> m_serviceUuids;
    QList<QLowEnergyService *> m_services;
    QList<CharacteristicInfo *> m_characteristics;
    bool m_connected;

signals:
    void connectionStatusChanged();

private slots:
    void connected();
    void disconnected();
    void error(QLowEnergyController::Error error);

    void serviceDiscovered(const QBluetoothUuid &serviceUuid);
    void serviceScanFinished();

    QString getServiceTypeString(QLowEnergyService *service);

    //QLowEnergyService
    void serviceStateChanged(QLowEnergyService::ServiceState state);
    void characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void confirmedDescriptorWrite(const QLowEnergyDescriptor &descriptor, const QByteArray &value);
    void serviceError(QLowEnergyService::ServiceError error);

    void discoverServices();
    void discoverService(QBluetoothUuid serviceUuid);
    void serviceCharacteristicsFound(QLowEnergyService *service);


public slots:

};

#endif // DEVICE_H
