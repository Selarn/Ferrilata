#include "message.h"
#include "ui_message.h"

Message::Message(TextMessageData *message, QWidget* parent) : QWidget(parent), ui(new Ui::Message) {
    ui->setupUi(this);
    ui->text->setText( message->get_text() );
    this->setStyleSheet( QString("border-radius: 3px; padding: 5px; color: ") + ( message->was_received() ? "#1565C0" : "#4CAF50" ) );
}

Message::~Message() {
    delete ui;
}
