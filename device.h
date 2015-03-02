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

#include "serviceinfo.h"
#include "characteristicinfo.h"

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(const QBluetoothDeviceInfo &deviceInfo, QObject *parent = 0);

    QString name() const;
    QBluetoothAddress address() const;
    bool isConnected() const;
    void discoverServices();
    void discoverService(QBluetoothUuid serviceUuid);
    QList<QBluetoothUuid> serviceUuids() const;

private:
    QBluetoothDeviceInfo m_deviceInfo;
    QLowEnergyController *m_controller;

    QList<QBluetoothUuid> m_serviceUuids;
    QList<ServiceInfo *> m_services;
    QList<CharacteristicInfo *> m_characteristics;
    bool m_connected;

signals:
    void connectionStatusChanged();
    void serviceScanningFinished();

private slots:
    void connected();
    void disconnected();
    void error(QLowEnergyController::Error error);

    void serviceDiscovered(const QBluetoothUuid &serviceUuid);
    void serviceStateChanged(QLowEnergyService::ServiceState newState);
    void serviceScanFinished();

    //QLowEnergyService
    void serviceCharacteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void serviceError(QLowEnergyService::ServiceError error);
    void confirmedDescriptorWrite(const QLowEnergyDescriptor &descriptor, const QByteArray &value);



public slots:

};

#endif // DEVICE_H
