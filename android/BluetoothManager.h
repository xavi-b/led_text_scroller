#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QBluetoothSocket>
#include <QtQml>

class BluetoothManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    BluetoothManager(QObject* parent = nullptr);
    Q_INVOKABLE void discoverDevices();
    void             connectToDevice(QBluetoothDeviceInfo const& device);
    Q_INVOKABLE void sendTextMessage(QString const& message);

private slots:
    void deviceDiscovered(QBluetoothDeviceInfo const& device);
    void discoveryFinished();
    void socketConnected();
    void socketDisconnected();
    void readSocket();

signals:
    void log(QString);

private:
    QBluetoothDeviceDiscoveryAgent* discoveryAgent;
    QBluetoothSocket*               socket;
};
