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
};

#endif // CONTROLLER_H