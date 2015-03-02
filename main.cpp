#include <QCoreApplication>
#include "core.h"

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    QCoreApplication::setApplicationName("bluetoothLeScanner");
    QCoreApplication::setApplicationVersion("0.0.1");

    QCommandLineParser *parser = new QCommandLineParser();
    parser->setApplicationDescription("Tool to inspect a bluetooth low energy device.");
    parser->addHelpOption();
    parser->addVersionOption();

    QCommandLineOption hostDeviceOption(QStringList() << "host", "The name of the Bluetooth host device to use. By default it will use the first host device found.", "hostname");
    parser->addOption(hostDeviceOption);

    QCommandLineOption discoverDevicesOption(QStringList() << "d" << "discover", "Search bluetooth devices.");
    parser->addOption(discoverDevicesOption);



    parser->process(application);

    Core core(parser);

    return application.exec();
}
