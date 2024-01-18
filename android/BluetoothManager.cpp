#include "BluetoothManager.h"

BluetoothManager::BluetoothManager(QObject* parent)
    : QObject(parent), discoveryAgent(new QBluetoothDeviceDiscoveryAgent(this)),
      socket(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this))
{
    connect(discoveryAgent,
            &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this,
            &BluetoothManager::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BluetoothManager::discoveryFinished);
    connect(socket, &QBluetoothSocket::connected, this, &BluetoothManager::socketConnected);
    connect(socket, &QBluetoothSocket::disconnected, this, &BluetoothManager::socketDisconnected);
    connect(socket, &QBluetoothSocket::readyRead, this, &BluetoothManager::readSocket);

    connect(this, &BluetoothManager::log, this, [](QString message) {
        qDebug() << message;
    });
}

void BluetoothManager::discoverDevices()
{
    emit log("Searching for paired Bluetooth devices...");
    discoveryAgent->stop();
    discoveryAgent->start();
}

void BluetoothManager::connectToDevice(QBluetoothDeviceInfo const& device)
{
    emit log("Connecting to device:" + device.name() + "(" + device.address().toString() + ")");
    socket->connectToService(device.address(), QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));
}

void BluetoothManager::sendTextMessage(QString const& message)
{
    if (socket->state() == QBluetoothSocket::SocketState::ConnectedState)
    {
        emit log("Sending message: " + message);
        socket->write(message.toUtf8() + '\n');
    }
    else
    {
        emit log("Not connected to any device.");
    }
}

void BluetoothManager::deviceDiscovered(QBluetoothDeviceInfo const& device)
{
    emit log("Found device: " + device.name() + "(" + device.address().toString() + ")");
    if (device.name() == "HC-05")
    {
        connectToDevice(device);
    }
}

void BluetoothManager::discoveryFinished()
{
    emit log("Device discovery finished.");
}

void BluetoothManager::socketConnected()
{
    emit log("Connected to the RFCOMM service.");
    discoveryAgent->stop();
}

void BluetoothManager::socketDisconnected()
{
    emit log("Disconnected from the RFCOMM service.");
}

void BluetoothManager::readSocket()
{
    emit log("Received data: " + socket->readAll());
}
