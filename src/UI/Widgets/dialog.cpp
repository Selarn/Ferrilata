#include "dialog.h"

void Dialog::add_message(MessageData* message) {
    if ( message->get_type() == MessageData::TEXT_MESSAGE ) {
        TextMessageData* text_message = (TextMessageData*)message;
        layout->addWidget( new Message(text_message, this) );
    }
}

QString Dialog::get_username() {
    return username;
}

Dialog::Dialog(const QString& username_, QWidget *parent) : QFrame(parent) {
    setAutoFillBackground( true );
    layout = new QVBoxLayout(this);
    username = username_;
}

Dialog::~Dialog() {
}
