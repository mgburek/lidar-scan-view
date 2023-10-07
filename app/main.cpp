#include <QApplication>
#include <QSurfaceFormat>

#include "scan_data_model.h"
#include "main_window.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    ScanDataModel scan_data_model;
    MainWindow main_window(&scan_data_model);
    ScannerController controller(&scan_data_model, &main_window);
    main_window.show();
    
    return app.exec();
}