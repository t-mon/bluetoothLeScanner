#include "characteristicinfo.h"

#include "qbluetoothuuid.h"
#include <QByteArray>

CharacteristicInfo::CharacteristicInfo()
{
}

CharacteristicInfo::CharacteristicInfo(const QLowEnergyCharacteristic &characteristic):
    m_characteristic(characteristic)
{
}

void CharacteristicInfo::setCharacteristic(const QLowEnergyCharacteristic &characteristic)
{
    m_characteristic = characteristic;
    emit characteristicChanged();
}

QString CharacteristicInfo::name() const
{
    QString name = m_characteristic.name();
    if (!name.isEmpty())
        return name;

    // find descriptor with CharacteristicUserDescription
    foreach (const QLowEnergyDescriptor &descriptor, m_characteristic.descriptors()) {
        if (descriptor.type() == QBluetoothUuid::CharacteristicUserDescription) {
            name = descriptor.value();
            break;
        }
    }

    if (name.isEmpty()) {
        name = "Unknown";
    }

    return name;
}

QBluetoothUuid CharacteristicInfo::uuid() const
{
    return m_characteristic.uuid();
}

QString CharacteristicInfo::hexUuid() const
{
    const QBluetoothUuid uuid = m_characteristic.uuid();
    bool success = false;
    quint16 result16 = uuid.toUInt16(&success);
    if (success)
        return QStringLiteral("0x") + QString::number(result16, 16);

    quint32 result32 = uuid.toUInt32(&success);
    if (success)
        return QStringLiteral("0x") + QString::number(result32, 16);

    return uuid.toString().remove(QLatin1Char('{')).remove(QLatin1Char('}'));
}

QByteArray CharacteristicInfo::value() const
{
    return m_characteristic.value();
}

QByteArray CharacteristicInfo::hexValue() const
{
    // Show raw string first and hex value below
    QByteArray a = m_characteristic.value();
    QByteArray result;
    if (a.isEmpty()) {
        result = "<none>";
        return result;
    }
    result = a.toHex();
    return result;
}

QString CharacteristicInfo::handle() const
{
    return QStringLiteral("0x") + QString::number(m_characteristic.handle(), 16);
}

QStringList CharacteristicInfo::permissions() const
{
    QStringList properties;
    int permission = m_characteristic.properties();
    if (permission & QLowEnergyCharacteristic::Read)
        properties.append("Read");
    if (permission & QLowEnergyCharacteristic::Write)
        properties.append("Write");
    if (permission & QLowEnergyCharacteristic::Notify)
        properties.append("Notify");
    if (permission & QLowEnergyCharacteristic::Indicate)
        properties.append("Indicate");
    if (permission & QLowEnergyCharacteristic::ExtendedProperty)
        properties.append("ExtendedProperty");
    if (permission & QLowEnergyCharacteristic::Broadcasting)
        properties.append("Broadcast");
    if (permission & QLowEnergyCharacteristic::WriteNoResponse)
        properties.append("WriteNoResp");
    if (permission & QLowEnergyCharacteristic::WriteSigned)
        properties.append("WriteSigned");

    return properties;
}

QLowEnergyCharacteristic CharacteristicInfo::characteristic() const
{
    return m_characteristic;
}
