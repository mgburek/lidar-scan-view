#include "controller.h"

#include <QInputDialog>

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
      _connection(mainWindow),
      _sampleCount{0},
      _expectedSampleCount{0}
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
    bool ok{false};
    float step =
        QInputDialog::getDouble(_mainWindow, "Scan precission", "Servo motor step",
                                1.0, 0.5, 10.0, 1, &ok, Qt::Widget, 0.1);
    if (ok)
    {
        _sampleCount = 0;
        _expectedSampleCount = static_cast<int>((1 + (_pitchRange / step)) * (1 + (_yawRange / step)));

        char code = static_cast<char>(MessageCode::STEP_INFO);
        _connection.write(&code, 1);

        // step * 10 should be maximally equal to 100 which fits into 8bits
        uint8_t step8byte = static_cast<uint8_t>(step * 10);
        _connection.write(reinterpret_cast<char *>(&step8byte), 1);

        code = static_cast<char>(MessageCode::START_SCANNING);
        _connection.write(&code, 1);
        _mainWindow->addLog("Scan started");
        _mainWindow->enableAction(ActionsFactory::Key::START_SCANNING, false);
    }
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
        if (bytes >= 8)
        {
            static char data[8];
            _connection.read(data, 8);

            int8_t beginCode = static_cast<int8_t>(data[0]);
            int8_t endCode = static_cast<int8_t>(data[7]);

            if (static_cast<MessageCode>(beginCode) == MessageCode::FRAME_BEGIN &&
                static_cast<MessageCode>(endCode) == MessageCode::FRAME_END)
            {
                int16_t distance = ((data[1] & 0x00ff) | (data[2] << 8 & 0xff00));
                int16_t upper = ((data[3] & 0x00ff) | (data[4] << 8 & 0xff00));
                int16_t bottom = ((data[5] & 0x00ff) | (data[6] << 8 & 0xff00));

                double yaw = bottom / 10.0;  // deg
                double pitch = upper / 10.0; // deg

                if (distance < 800 && distance > 20)
                {
                    _sampleCount++;
                    QVector3D cartesian = polarToCartesian(distance, yaw, pitch);
                    _scanDataModel->add(Point(cartesian.x(), cartesian.y(), cartesian.z(), distance / 200.0));
                    _mainWindow->updateScanVisualisation();
                }
                else
                    qDebug() << "Sample was thrown away";
            }
            else
            {
                for (int i{0}; i < sizeof(data); i++)
                    if (static_cast<MessageCode>(data[i]) == MessageCode::SCAN_FINISHED)
                    {
                        _mainWindow->addLog("Scan finished");
                        _mainWindow->addLog("Samples: " + QString::number(_sampleCount) + "/" +
                                            QString::number(_expectedSampleCount));
                        _mainWindow->enableAction(ActionsFactory::Key::START_SCANNING, true);
                        return;
                    }
            }

            for (int i{0}; i < sizeof(data); i++)
                data[i] = 0;
        }
    } while (bytes >= 8);
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