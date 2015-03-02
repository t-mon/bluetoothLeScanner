#include "bluetoothscanner.h"

BluetoothScanner::BluetoothScanner(QObject *parent) :
    QObject(parent)
{
}

bool BluetoothScanner::isAvailable(QString hostname)
{
    //Using default Bluetooth adapter
    QBluetoothLocalDevice localDevice;


    // Check if Bluetooth is available on this device
    if (!localDevice.isValid()) {
        qWarning() << "ERROR: no bluetooth device found.";
        exit(-1);
    }

    // Turn Bluetooth on
    localDevice.powerOn();

    // Make it visible to others
    localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

    // Get connected devices
    QList<QBluetoothHostInfo> remotes;
    remotes = localDevice.allDevices();

    if (remotes.isEmpty()) {
        qWarning() << "ERROR: no bluetooth host info found.";
        exit(-1);
    }


    QBluetoothAddress adapterAddress;
    qDebug() << "======================================";
    qDebug() << " Bluetooth Hosts: ";
    qDebug() << "---------------------------------------";
    foreach (QBluetoothHostInfo hostInfo, remotes) {
        if (hostInfo.name() == hostname) {
            adapterAddress = hostInfo.address();
        }
        qDebug() << "name :" << hostInfo.name();
        qDebug() << "mac  :" << hostInfo.address().toString();
        qDebug() << "======================================";
    }

    if (hostname.isEmpty()) {
        adapterAddress = remotes.first().address();
    }

    // Create a discovery agent and connect to its signals
    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(adapterAddress, this);

    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BluetoothScanner::deviceDiscovered);
    connect(m_discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)), this, SLOT(onError(QBluetoothDeviceDiscoveryAgent::Error)));

    return true;
}

void BluetoothScanner::startDiscovering()
{
    // Start a discovery
    qDebug() << "discovering...";
    qDebug() << "---------------------------------------";
    m_discoveryAgent->start();
}

void BluetoothScanner::stopDiscovering()
{
    // Start a discovery
    qDebug() << "stop discovering.";
    qDebug() << "---------------------------------------";
    m_discoveryAgent->stop();
}

QList<QBluetoothDeviceInfo> BluetoothScanner::deviceInfos() const
{
    return m_deviceInfos;
}

void BluetoothScanner::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    // check if this is LE device
    bool bluetoothLE = false;
    if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        bluetoothLE = true;
    }

    qDebug() << "device       : " << device.name();
    qDebug() << "mac          : " << device.address();
    qDebug() << "Bluetooth LE : " << bluetoothLE;
    qDebug() << "---------------------------------------";
    m_deviceInfos.append(device);

    emit deviceFound(device);
}

void BluetoothScanner::onError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    qWarning() << "ERROR: Bluetooth discovery" << error << m_discoveryAgent->errorString();
    exit(-1);
}

