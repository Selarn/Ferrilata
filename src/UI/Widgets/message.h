#ifndef MESSAGE_H
#define MESSAGE_H

#include "src/Network/responces.h"

#include <QWidget>


namespace Ui {
    class Message;
}


class Message : public QWidget {
    Q_OBJECT

public:
    explicit Message(TextMessageData* message , QWidget* parent);
    ~Message();

private:
    Ui::Message *ui;
};

#endif // MESSAGE_H
