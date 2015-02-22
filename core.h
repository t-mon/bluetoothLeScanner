#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "bluetoothscanner.h"
#include "device.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);

private:
    BluetoothScanner *m_bluetoothScanner;

signals:

private slots:
    void deviceFound(const QBluetoothDeviceInfo &deviceInfo);


};

#endif // CORE_H
