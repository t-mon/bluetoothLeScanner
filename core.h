#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "bluetoothscanner.h"
#include "device.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QCommandLineParser *parser, QObject *parent = 0);

private:
    BluetoothScanner *m_bluetoothScanner;
    Device *m_device;
    QCommandLineParser *m_parser;

signals:

private slots:
    void deviceFound(const QBluetoothDeviceInfo &deviceInfo);
    void deviceConnectionStateChanged();
    void serviceScanningFinished();

};

#endif // CORE_H
