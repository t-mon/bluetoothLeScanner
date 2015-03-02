#ifndef BLUETOOTHSCANNER_H
#define BLUETOOTHSCANNER_H

#include <QObject>
#include <QDebug>
#include <QtBluetooth>
#include <QBluetoothHostInfo>
#include <QBluetoothDeviceInfo>
#include <QBluetoothAddress>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>

class BluetoothScanner : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothScanner(QObject *parent = 0);
    bool isAvailable(QString hostname = QString());
    void startDiscovering();
    void stopDiscovering();

    QList<QBluetoothDeviceInfo> deviceInfos() const;

private:
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QList<QBluetoothDeviceInfo> m_deviceInfos;


signals:
    void deviceFound(const QBluetoothDeviceInfo &device);

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void onError(QBluetoothDeviceDiscoveryAgent::Error error);
};

#endif // BLUETOOTHSCANNER_H
