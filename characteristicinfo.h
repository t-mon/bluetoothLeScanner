#ifndef CHARACTERISTICINFO_H
#define CHARACTERISTICINFO_H

#include <QObject>
#include <QLowEnergyCharacteristic>
#include <QBluetoothUuid>

class CharacteristicInfo: public QObject
{
    Q_OBJECT
public:
    CharacteristicInfo();
    CharacteristicInfo(const QLowEnergyCharacteristic &characteristic);
    void setCharacteristic(const QLowEnergyCharacteristic &characteristic);
    QString name() const;
    QBluetoothUuid uuid() const;
    QString hexUuid() const;
    QByteArray value() const;
    QByteArray hexValue() const;
    QString handle() const;
    QStringList permissions() const;
    QLowEnergyCharacteristic characteristic() const;

private:
    QLowEnergyCharacteristic m_characteristic;

signals:
    void characteristicChanged();

};

#endif // CHARACTERISTICINFO_H
