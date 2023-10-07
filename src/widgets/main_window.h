#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include <QToolBar>

#include "scan_data_model.h"

#include "menu_bar.h"
#include "opengl_view.h"
#include "logs_view.h"
#include "status_bar.h"
#include "actions.h"

// #   #   ###   #  #   #     #   #  #  #   #  ####   #####  #   #  
// ## ##  #   #  #  ##  #     #   #  #  ##  #  #   #  #   #  #   #  
// # # #  #####  #  # # #     # # #  #  # # #  #   #  #   #  # # #  
// #   #  #   #  #  #  ##     # # #  #  #  ##  #   #  #   #  # # #  
// #   #  #   #  #  #   #      # #   #  #   #  ####   #####   # #  

class MainWindow : public QWidget
{
public:
    MainWindow(ScanDataModel *scanDataModel, QWidget *parent = nullptr);

private:
    void toggleLogsViewVisibility();
    void clearVisualistion();

    void saveToFile();
    void loadFromFile();

public:
    void setActionCallback(ActionsFactory::Key key, std::function<void()> callback);

    void showAction(ActionsFactory::Key key, bool show);
    void enableAction(ActionsFactory::Key key, bool enable);

    void updateScanVisualisation() { _openGLView->update(); }

    void addLog(QString message);
    void statusBarMessage(StatusBar::Message message);

private:
    void initUI();

private:
    ScanDataModel *_scanDataModel;

    MenuBar *_menuBar;
    QToolBar *_toolBar;
    OpenGLView *_openGLView;
    LogsView *_logsView;
    StatusBar *_statusBar;
    ActionsFactory _actionsFactory;
};

#endif // MAIN_WINDOW_H