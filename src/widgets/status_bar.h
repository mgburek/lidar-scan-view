#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include <QStatusBar>

// #####  #####   ###   #####  #   #  #####     ####    ###   ####   
// #        #    #   #    #    #   #  #         #   #  #   #  #   #  
// #####    #    #####    #    #   #  #####     ####   #####  ####   
//     #    #    #   #    #    #   #      #     #   #  #   #  #  #   
// #####    #    #   #    #     ###   #####     ####   #   #  #   #  

class StatusBar : public QStatusBar
{
public:
    enum class Message
    {
        NO_DEVICE_CONNECTED,
        DEVICE_CONNECTED
    };

    StatusBar(QWidget *parent)
    {
        setMessage(Message::NO_DEVICE_CONNECTED);
    };

    void setMessage(Message message)
    {
        this->showMessage(_messagesMap[message], 0);
    }

private:
    inline static const QMap<Message, QString> _messagesMap{
        {Message::DEVICE_CONNECTED, "Device is connected"},
        {Message::NO_DEVICE_CONNECTED, "Device is NOT connected"}};
};

#endif // STATUS_BAR_H