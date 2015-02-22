#include "core.h"

Core::Core(QObject *parent) :
    QObject(parent)
{
    m_bluetoothScanner = new BluetoothScanner(this);

    connect(m_bluetoothScanner, &BluetoothScanner::deviceFound, this, &Core::deviceFound);

    if (m_bluetoothScanner->isAvailable()) {
        m_bluetoothScanner->startDiscovering();
    }
}

void Core::deviceFound(const QBluetoothDeviceInfo &deviceInfo)
{
    if (deviceInfo.name() == "iSmartLight Bough") {
        qDebug() << "Device found:";
        qDebug() << "---------------------------------------";
        qDebug() << "name : " << deviceInfo.name();
        qDebug() << "mac  : " << deviceInfo.address();
        qDebug() << "---------------------------------------";
        m_bluetoothScanner->stopDiscovering();
        Device *device = new Device(deviceInfo, this);
    }
}
