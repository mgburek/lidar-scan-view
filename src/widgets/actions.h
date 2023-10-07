#include <QAction>

//  ###   #####  #####  #  #####  #   #
// #   #  #        #    #  #   #  ##  #
// #####  #        #    #  #   #  # # #
// #   #  #        #    #  #   #  #  ##
// #   #  #####    #    #  #####  #   #

class Action final : public QAction
{
public:
    Action(QObject *parent, QString name, QString menuName, bool isVisibleOnToolbar = true)
        : QAction(name, nullptr), _menuName{menuName}, _isVisibleOnToolbar{isVisibleOnToolbar}
    {
    }

    Action() : Action(nullptr, "", "") {}

    bool isVisibleOnToolbar() const { return _isVisibleOnToolbar; }
    bool isVisibleInMenus() const { return !_menuName.isEmpty(); }
    QString menuName() const { return _menuName; }

private:
    QString _menuName;
    bool _isVisibleOnToolbar;
};

//  ###   #####  #####  #  #####  #   #  #####     #####   ###   #####  #####  #####  ####   #   #  
// #   #  #        #    #  #   #  ##  #  #         #      #   #  #        #    #   #  #   #   # #   
// #####  #        #    #  #   #  # # #  #####     #####  #####  #        #    #   #  ####     #    
// #   #  #        #    #  #   #  #  ##      #     #      #   #  #        #    #   #  #  #     #    
// #   #  #####    #    #  #####  #   #  #####     #      #   #  #####    #    #####  #   #    #   

class ActionsFactory
{
public:
    enum class Key
    {
        SAVE_SCAN,
        LOAD_SCAN_FROM_FILE,
        TOGGLE_LOGS,
        CLEAR_SCREEN,
        CONNECT_TO_DEVICE,
        DISCONNECT_DEVICE,
        START_SCANNING,
        STOP_ALL
    };

    ActionsFactory(QObject *parent)
    {
        init(parent);
    }

    Action *get(Key key)
    {
        if (_actionsMap.find(key) == _actionsMap.end())
            return nullptr;
        return _actionsMap[key];
    }

    QMap<Key, Action *> &getAll()
    {
        return _actionsMap;
    }

private:
    void init(QObject *parent)
    {
        _actionsMap =
            QMap<Key, Action *>(
                {{Key::SAVE_SCAN, new Action(parent, "Save", "File", false)},
                 {Key::LOAD_SCAN_FROM_FILE, new Action(parent, "Load file", "File", false)},
                 {Key::DISCONNECT_DEVICE, new Action(parent, "Disconnect", "Device")},
                 {Key::CONNECT_TO_DEVICE, new Action(parent, "Connect", "Device")},
                 {Key::START_SCANNING, new Action(parent, "Start scanning", "Device")},
                 {Key::STOP_ALL, new Action(parent, "Stop all", "Device")},
                 {Key::TOGGLE_LOGS, new Action(parent, "Toggle logs", "View")},
                 {Key::CLEAR_SCREEN, new Action(parent, "Clear screen", "View")}});
    }

private:
    QMap<Key, Action *> _actionsMap;
};