#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <QMenuBar>

// #   #  #####  #   #  #   #     ####    ###   ####   
// ## ##  #      ##  #  #   #     #   #  #   #  #   #  
// # # #  #####  # # #  #   #     ####   #####  ####   
// #   #  #      #  ##  #   #     #   #  #   #  #  #   
// #   #  #####  #   #   ###      ####   #   #  #   # 

class MenuBar : public QMenuBar
{
public:
    MenuBar(QWidget *parent) : _menusMap{QMap<QString, QMenu *>()} {}

    void addActionToMenu(QString menuName, QAction *action)
    {
        if (_menusMap.find(menuName) == _menusMap.end())
            _menusMap[menuName] = new QMenu(menuName, this);
        _menusMap[menuName]->addAction(action);
        updateMenus();
    }

private:
    void updateMenus()
    {
        this->clear();
        for (QMenu *menu : _menusMap)
        {
            this->addMenu(menu);
        }
    }

private:
    QMap<QString, QMenu *> _menusMap;
};

#endif // MENU_BAR_H