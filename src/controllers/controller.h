#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "scan_data_model.h"
#include "main_window.h"

// #####  #####  #   #  #####  ####   #####  #      #      #####  ####
// #      #   #  ##  #    #    #   #  #   #  #      #      #      #   #
// #      #   #  # # #    #    ####   #   #  #      #      #####  ####
// #      #   #  #  ##    #    #  #   #   #  #      #      #      #  #
// #####  #####  #   #    #    #   #  #####  #####  #####  #####  #   #

class ScannerController : public QObject
{
    Q_OBJECT

    enum class MessageCode : int8_t
    {
        UNKNOWN = 0x60,
        STOP_ALL = 0x61,
        START_SCANNING = 0x62,
        OK = 0x63,
        FRAME_BEGIN = 0x64,
        FRAME_END = 0x65,
        SCAN_FINISHED = 0x66,
        STEP_INFO = 0x67
    };

public:
    ScannerController(ScanDataModel *scanDataModel, MainWindow *mainWindow);
    ~ScannerController();

private slots:
    void connectToDevice();
    void disconnectDevice();
    void startScanning();
    void stopAll();
    void readScanData();
    void handleConnectionError(QSerialPort::SerialPortError error);

private:
    ScanDataModel *_scanDataModel;
    MainWindow *_mainWindow;

    QSerialPort _connection;
    long int _sampleCount;
    long int _expectedSampleCount;
    inline static const float _yawRange = 180.0;
    inline static const float _pitchRange = 120.0;
};

#endif // CONTROLLER_H