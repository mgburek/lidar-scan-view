#include "main_window.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>

// #   #   ###   #  #   #     #   #  #  #   #  ####   #####  #   #
// ## ##  #   #  #  ##  #     #   #  #  ##  #  #   #  #   #  #   #
// # # #  #####  #  # # #     # # #  #  # # #  #   #  #   #  # # #
// #   #  #   #  #  #  ##     # # #  #  #  ##  #   #  #   #  # # #
// #   #  #   #  #  #   #      # #   #  #   #  ####   #####   # #

MainWindow::MainWindow(ScanDataModel *scanDataModel, QWidget *parent)
    : QWidget(parent),
      _scanDataModel{scanDataModel},
      _menuBar{new MenuBar(this)},
      _toolBar{new QToolBar(this)},
      _openGLView{new OpenGLView(this, scanDataModel)},
      _logsView{new LogsView(this)},
      _statusBar{new StatusBar(this)},
      _actionsFactory(this)
{
    initUI();

    setActionCallback(ActionsFactory::Key::TOGGLE_LOGS, [this]()
                      { this->toggleLogsViewVisibility(); });

    setActionCallback(ActionsFactory::Key::CLEAR_SCREEN, [this]()
                      { this->clearVisualistion(); });

    setActionCallback(ActionsFactory::Key::SAVE_SCAN, [this]()
                      { this->saveToFile(); });

    setActionCallback(ActionsFactory::Key::LOAD_SCAN_FROM_FILE, [this]()
                      { this->loadFromFile(); });
}

void MainWindow::initUI()
{
    QVBoxLayout *main_layout{new QVBoxLayout(this)};
    {
        main_layout->addWidget(_menuBar);
        main_layout->addWidget(_toolBar);

        main_layout->addWidget(_openGLView);
        {
            QHBoxLayout *middle_layout{new QHBoxLayout(_openGLView)};
            middle_layout->addSpacerItem(
                new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Expanding));
            middle_layout->addWidget(_logsView);
            middle_layout->setSpacing(0);
        }
        main_layout->addWidget(_statusBar);
        main_layout->setContentsMargins(0, 0, 0, 0);
    }

    auto actions = _actionsFactory.getAll();
    for (auto it = actions.begin(); it != actions.end(); it++)
    {
        Action *action = it.value();
        ActionsFactory::Key key = it.key();

        _menuBar->addActionToMenu(action->menuName(), action);

        if (action->isVisibleOnToolbar())
            _toolBar->addAction(action);
    }

    actions[ActionsFactory::Key::DISCONNECT_DEVICE]->setVisible(false);
    actions[ActionsFactory::Key::START_SCANNING]->setEnabled(false);
    actions[ActionsFactory::Key::STOP_ALL]->setEnabled(false);
}

void MainWindow::setActionCallback(ActionsFactory::Key key, std::function<void()> callback)
{
    if (auto a = _actionsFactory.get(key))
    {
        connect(a, &QAction::triggered, callback);
    }
    else
        qDebug() << "nullptr";
}

void MainWindow::showAction(ActionsFactory::Key key, bool enable)
{
    _actionsFactory.get(key)->setVisible(enable);
}

void MainWindow::enableAction(ActionsFactory::Key key, bool enable)
{
    _actionsFactory.get(key)->setEnabled(enable);
}

void MainWindow::toggleLogsViewVisibility()
{
    _logsView->isVisible() ? _logsView->hide() : _logsView->show();
}

void MainWindow::clearVisualistion()
{
    _scanDataModel->removeAllScans();
    _openGLView->update();
    addLog("Scan deleted");
}

void MainWindow::addLog(QString message)
{
    _logsView->addLog(message);
}

void MainWindow::statusBarMessage(StatusBar::Message message)
{
    _statusBar->setMessage(message);
}

void MainWindow::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save to a file", "C:/", "(*.scan)");
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        addLog("Couldn't save to a file");
    }

    for (const Point &point : _scanDataModel->points())
    {
        float x = point.x();
        float y = point.y();
        float z = point.z();
        float s = point.scale();

        file.write(reinterpret_cast<char *>(&x), sizeof(float));
        file.write(reinterpret_cast<char *>(&y), sizeof(float));
        file.write(reinterpret_cast<char *>(&z), sizeof(float));
        file.write(reinterpret_cast<char *>(&s), sizeof(float));
    }

    file.close();

    addLog("Scan was saved to " + fileName);
}

void MainWindow::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open a file", "C:/", "(*.scan)");
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        addLog("Couldn't load a file");
    }

    this->clearVisualistion();

    while (!file.atEnd())
    {
        float x, y, z, s;

        file.read(reinterpret_cast<char *>(&x), sizeof(float));
        file.read(reinterpret_cast<char *>(&y), sizeof(float));
        file.read(reinterpret_cast<char *>(&z), sizeof(float));
        file.read(reinterpret_cast<char *>(&s), sizeof(float));

        _scanDataModel->add(Point(x, y, z, s));
    }

    file.close();

    addLog("Scan was loaded from " + fileName);
}