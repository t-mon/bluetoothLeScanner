#include "core.h"

Core::Core(QCommandLineParser *parser, QObject *parent) :
    m_parser(parser), QObject(parent)
{

    foreach (QString option, m_parser->optionNames()) {
        if (option == "host") {

        }
    }

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

        m_device = new Device(deviceInfo, this);
        connect(m_device, &Device::connectionStatusChanged, this, &Core::deviceConnectionStateChanged);
        connect(m_device, &Device::serviceScanningFinished, this, &Core::serviceScanningFinished);
    }
}

void Core::deviceConnectionStateChanged()
{
    if (m_device->isConnected()) {

    }
}

void Core::serviceScanningFinished()
{

}
