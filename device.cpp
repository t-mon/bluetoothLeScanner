#include "device.h"

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

    if (!m_deviceInfo.isValid()) {
        qWarning() << "ERROR: Device not valid";
    }

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
    m_controller->discoverServices();
}

void Device::disconnected()
{
    m_connected = false;
    emit connectionStatusChanged();

    qWarning() << "Device" << address().toString() << "disconnected";
    exit(0);
}

void Device::error(QLowEnergyController::Error error)
{
    qWarning() << "ERROR: Device" << name() << address().toString() << ": " << error << m_controller->errorString();
}

void Device::serviceDiscovered(const QBluetoothUuid &serviceUuid)
{
    QLowEnergyService *service = m_controller->createServiceObject(serviceUuid);

    if (!service) {
        qWarning() << "Cannot create service for uuid";
        return;
    }

    connect(service, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)), this, SLOT(serviceCharacteristicChanged(QLowEnergyCharacteristic,QByteArray)));
    connect(service, SIGNAL(error(QLowEnergyService::ServiceError)), this, SLOT(serviceError(QLowEnergyService::ServiceError)));
    connect(service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)), this, SLOT(serviceStateChanged(QLowEnergyService::ServiceState)));

    ServiceInfo *serviceInfo = new ServiceInfo(service);
    m_services.append(serviceInfo);

    qDebug() << "----------------------------------------------------------------";
    qDebug() << "name           :" << serviceInfo->name();
    qDebug() << "type           :" << serviceInfo->type();
    qDebug() << "uuid           :" << serviceInfo->uuid();
    qDebug() << "uuid (hex)     :" << serviceInfo->uuidHex();
}

void Device::serviceStateChanged(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::ServiceDiscovered) {
        return;
    }

    // get service
    QLowEnergyService *service = qobject_cast<QLowEnergyService *>(sender());

    qDebug() << "----------------------------------------------------------------";
    qDebug() << "characteristics:";
    foreach (QLowEnergyCharacteristic characteristic, service->characteristics()) {
        CharacteristicInfo *info = new CharacteristicInfo(characteristic);
        qDebug() << "   name            :" << info->name();
        qDebug() << "   uuid            :" << info->uuid().toString();
        qDebug() << "   uuid (hex)      :" << info->hexUuid();
        qDebug() << "   handle          :" << info->handle();
        qDebug() << "   permission      :" << info->permissions();
        qDebug() << "   value           :" << info->value();
        qDebug() << "   value (hex)     :" << info->value().toHex();
        qDebug() << "   ---------------------------------------------------------";
        qDebug() << "   descriptor count:" << characteristic.descriptors().count();
        qDebug() << "   ---------------------------------------------------------";
        if (!characteristic.descriptors().isEmpty()) {
            foreach (QLowEnergyDescriptor descriptor, characteristic.descriptors()) {
                if (descriptor.isValid()) {
                    qDebug() << "       name        :" << descriptor.name();
                    qDebug() << "       uuid        :" << descriptor.uuid();
                    qDebug() << "       handle      :" << descriptor.handle();
                    qDebug() << "       value       :" << descriptor.value();
                    qDebug() << "       value (hex) :" << descriptor.value().toHex();
                    qDebug() << "       -----------------------------------------------------";
                } else {
                    qWarning() << "desciptor is not valid";
                }
            }
        }

        m_characteristics.append(info);
    }
    qDebug() << "Done discovering details for service"  << service->serviceName() ;;
    qDebug() << "================================================================";
}

void Device::serviceScanFinished()
{
    qDebug() << "----------------------------------------------------------------";
    qDebug() << "\nservice scan finished. (found "<< m_services.count() << "services) \n ";

    foreach (ServiceInfo *serviceInfo, m_services) {

        // check unkown service
        if (serviceInfo->uuid().toString() == "{0000fff0-0000-1000-8000-00805f9b34fb}") {
            qDebug() << "-> get details of service " << serviceInfo->name() << serviceInfo->uuid().toString();

            if (serviceInfo->service()->state() == QLowEnergyService::IncludedService) {
                qDebug() << "seems to be a included service";
            }

            if (serviceInfo->service()->state() == QLowEnergyService::InvalidService) {
                qDebug() << "seems to be a invalid service";
            }

            // check if we need to discover the details of this service
            if (serviceInfo->service()->state() == QLowEnergyService::DiscoveryRequired) {
                qDebug() << "service needs discovery!";
                serviceInfo->service()->discoverDetails();
                qDebug() << "discovering...";
                return;
            }
        }
    }
}

void Device::serviceCharacteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    qDebug() << characteristic.name() << value << "changed!!!";
}

void Device::confirmedDescriptorWrite(const QLowEnergyDescriptor &descriptor, const QByteArray &value)
{
    qDebug() << "confirm descriptor write: " << descriptor.name() << value;
}

void Device::serviceError(QLowEnergyService::ServiceError error)
{
    QLowEnergyService *service = qobject_cast<QLowEnergyService *>(sender());
    qWarning() << "ERROR" << service->serviceName() << service->serviceUuid() << error;
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

    // check if we need to discover the details of this service
    if (service->state() == QLowEnergyService::DiscoveryRequired) {
        connect(service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)), this, SLOT(serviceStateChanged(QLowEnergyService::ServiceState)));
        service->discoverDetails();
        return;
    }
}

QList<QBluetoothUuid> Device::serviceUuids() const
{
    return m_serviceUuids;
}

