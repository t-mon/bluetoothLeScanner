#include "device.h"

QBluetoothUuid lumenUuid = QBluetoothUuid(QString("0000180a-0000-1000-8000-00805f9b34fb"));

Device::Device(const QBluetoothDeviceInfo &deviceInfo, QObject *parent) :
    m_deviceInfo(deviceInfo), QObject(parent)
{
    m_controller = new QLowEnergyController(address(), this);

    connect(m_controller, &QLowEnergyController::connected, this, &Device::connected);
    connect(m_controller, &QLowEnergyController::disconnected, this, &Device::disconnected);
    connect(m_controller, SIGNAL(error(QLowEnergyController::Error)), this, SLOT(error(QLowEnergyController::Error)));
    connect(m_controller, &QLowEnergyController::serviceDiscovered, this, &Device::serviceDiscovered);
    connect(m_controller, SIGNAL(discoveryFinished()), this, SLOT(serviceScanFinished()));
    m_controller->setRemoteAddressType(QLowEnergyController::PublicAddress);

    m_controller->connectToDevice();
}

QString Device::name() const
{
    return m_deviceInfo.name();
}

QBluetoothAddress Device::address() const
{
    return m_deviceInfo.address();
}

bool Device::isConnected() const
{
    return m_connected;
}

void Device::connected()
{
    qDebug() << "Connected successfully to " << address().toString();
    m_connected = true;
    emit connectionStatusChanged();

    qDebug() << "scan services...";
    m_controller->discoverServices();
}

void Device::disconnected()
{
    m_connected = false;
    emit connectionStatusChanged();

    qWarning() << "Device" << address().toString() << "disconnected";
    exit(0);
    m_controller->connectToDevice();
}

void Device::error(QLowEnergyController::Error error)
{
    qWarning() << "ERROR: Device" << name() << address().toString() << ": " << error << m_controller->errorString();
}

void Device::serviceDiscovered(const QBluetoothUuid &serviceUuid)
{
    qDebug() << " --> service discoverd: " << serviceUuid.toString();
    m_serviceUuids.append(serviceUuid);
}

void Device::serviceScanFinished()
{
    qDebug() << "service scan finished.";

    discoverServices();
}

QString Device::getServiceTypeString(QLowEnergyService *service)
{
    QString typeString;
    if (service->type() & QLowEnergyService::PrimaryService)
        typeString = "primary";
    else
        typeString = "secondary";

    if (service->type() & QLowEnergyService::IncludedService)
        typeString = " included";

    return typeString;
}

void Device::serviceStateChanged(QLowEnergyService::ServiceState state)
{
    QLowEnergyService *service = qobject_cast<QLowEnergyService *>(sender());

    switch (state) {
    case QLowEnergyService::DiscoveringServices:
        qDebug() << "Start scanning service" << service->serviceName() << "details";
        return;
    case QLowEnergyService::ServiceDiscovered: {

        serviceCharacteristicsFound(service);

        foreach (QBluetoothUuid uuid, service->includedServices()) {
            qDebug() << "have include service" << uuid.toString();
        }
        return;
    }
    case QLowEnergyService::InvalidService:
        qDebug() << "Invalid service: " << service->serviceName() << service->serviceUuid();
    }
}

void Device::characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    qDebug() << characteristic.name() << value;
}


void Device::confirmedDescriptorWrite(const QLowEnergyDescriptor &descriptor, const QByteArray &value)
{

}

void Device::serviceError(QLowEnergyService::ServiceError error)
{

}

void Device::discoverServices()
{
    if (m_serviceUuids.isEmpty()) {
        return;
    }

    QBluetoothUuid serviceUuid = m_serviceUuids.takeFirst();
    QLowEnergyService *service = m_controller->createServiceObject(serviceUuid);

    if (!service) {
        qWarning() << "ERROR: Cannot create service for " << serviceUuid;
        return;
    }

    m_services.append(service);

    // check if we need to discover the details of this service
    if (service->state() == QLowEnergyService::DiscoveryRequired) {
        connect(service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)), this, SLOT(serviceStateChanged(QLowEnergyService::ServiceState)));
        service->discoverDetails();
        return;
    }

}

void Device::discoverService(QBluetoothUuid serviceUuid)
{

}

void Device::serviceCharacteristicsFound(QLowEnergyService *service)
{
    qDebug() << "----------------------------------------------------------------";
    qDebug() << "name:" << service->serviceName();
    qDebug() << "uuid:" << service->serviceUuid();
    qDebug() << "type:" << getServiceTypeString(service);

    qDebug() << "----------------------------------------------------------------";
    qDebug() << "characteristics:";
    foreach (QLowEnergyCharacteristic characteristic, service->characteristics()) {
        CharacteristicInfo info(characteristic);
        qDebug() << "   name       :" << info.name();
        qDebug() << "   uuid       :" << info.uuid().toString();
        qDebug() << "   uuid (hex) :" << info.hexUuid();
        qDebug() << "   handle     :" << info.handle();
        qDebug() << "   permission :" << info.permissions();
        qDebug() << "   value      :" << info.value();
        qDebug() << "   value (hex):" << info.value();
        qDebug() << "---------------------";
    }
    qDebug() << "Done discovering service "  << service->serviceName() << "details";;
    qDebug() << "================================================================";

    discoverServices();
}

