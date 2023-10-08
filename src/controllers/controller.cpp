#include "controller.h"

// #####  #####  #   #  #####  ####   #####  #      #      #####  ####
// #      #   #  ##  #    #    #   #  #   #  #      #      #      #   #
// #      #   #  # # #    #    ####   #   #  #      #      #####  ####
// #      #   #  #  ##    #    #  #   #   #  #      #      #      #  #
// #####  #####  #   #    #    #   #  #####  #####  #####  #####  #   #

namespace
{
    QVector3D polarToCartesian(double radius, double yaw, double pitch)
    {
        double rad = M_PI / 180.0;

        double z = radius * std::sin(pitch * rad);
        double x = radius * std::cos(pitch * rad) * std::cos(yaw * rad);
        double y = -radius * std::cos(pitch * rad) * std::sin(yaw * rad);

        return QVector3D(-x, z, -y);
    }
} // namespace

ScannerController::ScannerController(ScanDataModel *scanDataModel, MainWindow *mainWindow)
    : QObject(),
      _scanDataModel{scanDataModel},
      _mainWindow{mainWindow},
      _connection(mainWindow)
{

    _mainWindow->setActionCallback(ActionsFactory::Key::CONNECT_TO_DEVICE, [this]()
                                   { this->connectToDevice(); });

    _mainWindow->setActionCallback(ActionsFactory::Key::DISCONNECT_DEVICE, [this]()
                                   { this->disconnectDevice(); });

    _mainWindow->setActionCallback(ActionsFactory::Key::START_SCANNING, [this]()
                                   { this->startScanning(); });

    _mainWindow->setActionCallback(ActionsFactory::Key::STOP_ALL, [this]()
                                   { this->stopAll(); });

    QObject::connect(&_connection, &QSerialPort::readyRead, this, &ScannerController::readScanData);
    QObject::connect(&_connection, &QSerialPort::errorOccurred, this, &ScannerController::handleConnectionError);
}

ScannerController::~ScannerController()
{
    _connection.close();
}

void ScannerController::connectToDevice()
{
    QSerialPortInfo port;
    QString msg{};

    QList<QSerialPortInfo> ports_info = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &port_info : ports_info)
    {
        if (port_info.description() == "STMicroelectronics STLink Virtual COM Port")
            ;
        {
            port = port_info;
            break;
        }
    }
    if (port.portName() == "")
    {
        msg = "Device not found";
        _mainWindow->statusBarMessage(StatusBar::Message::NO_DEVICE_CONNECTED);
        _mainWindow->addLog(msg);

        return;
    }

    msg = "Device found";
    _mainWindow->addLog(msg);

    _connection.setPort(port);
    _connection.setBaudRate(QSerialPort::Baud115200);
    bool success = _connection.open(QIODeviceBase::ReadWrite);

    if (success)
    {
        msg = "Connected successfully";
        _mainWindow->statusBarMessage(StatusBar::Message::DEVICE_CONNECTED);
        _mainWindow->addLog(msg);

        _mainWindow->showAction(ActionsFactory::Key::DISCONNECT_DEVICE, true);
        _mainWindow->showAction(ActionsFactory::Key::CONNECT_TO_DEVICE, false);

        _mainWindow->enableAction(ActionsFactory::Key::START_SCANNING, true);
        _mainWindow->enableAction(ActionsFactory::Key::STOP_ALL, true);

        return;
    }
    else
    {
        msg = "Connection failed: " + _connection.error();
        _mainWindow->statusBarMessage(StatusBar::Message::NO_DEVICE_CONNECTED);
        _mainWindow->addLog(msg);
        return;
    }
}

void ScannerController::disconnectDevice()
{
    QString msg{};

    if (!_connection.isOpen())
        msg = "No device is connected";
    else
    {
        msg = "Disconnecting device";
        _connection.close();

        _mainWindow->showAction(ActionsFactory::Key::DISCONNECT_DEVICE, false);
        _mainWindow->showAction(ActionsFactory::Key::CONNECT_TO_DEVICE, true);

        _mainWindow->enableAction(ActionsFactory::Key::START_SCANNING, false);
        _mainWindow->enableAction(ActionsFactory::Key::STOP_ALL, false);

        _mainWindow->statusBarMessage(StatusBar::Message::NO_DEVICE_CONNECTED);
    }
    _mainWindow->addLog(msg);
}

void ScannerController::startScanning()
{
    char code = 0x62;
    _connection.write(&code, 1);
    _mainWindow->addLog("Scan started");
}

void ScannerController::stopAll()
{
    char code = 0x61;
    _connection.write(&code, 1);
    _mainWindow->addLog("Device was stopped by user");
}

void ScannerController::readScanData()
{
    int bytes{0};
    do
    {
        bytes = _connection.bytesAvailable();
        if (bytes >= 6)
        {
            static char data[6];
            _connection.read(data, 6);

            int16_t distance = ((data[0] & 0x00ff) | (data[1] << 8 & 0xff00));
            int16_t upper = ((data[2] & 0x00ff) | (data[3] << 8 & 0xff00));
            int16_t bottom = ((data[4] & 0x00ff) | (data[5] << 8 & 0xff00));

            double yaw = bottom / 10.0;  // deg
            double pitch = upper / 10.0; // deg

            QVector3D cartesian = polarToCartesian(distance, yaw, pitch);
            if (distance < 800 && distance > 20)
            {
                _scanDataModel->add(Point(cartesian.x(), cartesian.y(), cartesian.z(), distance / 200.0));
                _mainWindow->updateScanVisualisation();
            }

            for (int i{0}; i < sizeof(data);)
                data[i++] = 0;
        }
    } while (bytes >= 6);
}

void ScannerController::handleConnectionError(QSerialPort::SerialPortError error)
{
    switch (error)
    {
    case QSerialPort::SerialPortError::NoError:
    case QSerialPort::SerialPortError::DeviceNotFoundError:
    case QSerialPort::SerialPortError::NotOpenError:
    case QSerialPort::SerialPortError::OpenError:
    case QSerialPort::SerialPortError::PermissionError:
    case QSerialPort::SerialPortError::ReadError:
    case QSerialPort::SerialPortError::TimeoutError:
    case QSerialPort::SerialPortError::UnknownError:
    case QSerialPort::SerialPortError::UnsupportedOperationError:
    case QSerialPort::SerialPortError::WriteError:
        return;
    case QSerialPort::SerialPortError::ResourceError:
        _connection.close();

        _mainWindow->showAction(ActionsFactory::Key::DISCONNECT_DEVICE, false);
        _mainWindow->showAction(ActionsFactory::Key::CONNECT_TO_DEVICE, true);

        _mainWindow->enableAction(ActionsFactory::Key::START_SCANNING, false);
        _mainWindow->enableAction(ActionsFactory::Key::STOP_ALL, false);

        _mainWindow->statusBarMessage(StatusBar::Message::NO_DEVICE_CONNECTED);

        _mainWindow->addLog("Connection lost");
        break;
    }
}