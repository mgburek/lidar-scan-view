#ifndef LOGS_VIEW_H
#define LOGS_VIEW_H

#include <QListWidget>
#include <QTime>

// #      #####  #####  #####     #   #  #  #####  #   #  
// #      #   #  #      #         #   #  #  #      #   #  
// #      #   #  #  ##  #####     #   #  #  #####  # # #  
// #      #   #  #   #      #      # #   #  #      # # #  
// #####  #####  #####  #####       #    #  #####   # # 

class LogsView : public QListWidget
{
public:
    LogsView(QWidget *parent = nullptr)
    {
        this->setFixedWidth(350);
        this->setAutoScroll(true);
        this->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
    }

    void addLog(QString log)
    {
        this->addItem(QTime::currentTime().toString() + " - " + log);
    }
};

#endif // LOGS_VIEW_H