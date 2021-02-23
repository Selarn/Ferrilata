#include "client.h"

Client::Client(QString ip, qint16 port) : QObject(0) {
    show_preview();

    gui = new GUI(this);
    network = new Network( QHostAddress(ip), port );
    settings = new Settings();

    connect( network, &Network::message_recived,   gui, &GUI::add_message );
    connect( network, &Network::dialog_viewed,     gui, &GUI::dialog_viewed );
    connect( network, &Network::message_delivered, gui, &GUI::message_delivered );
    connect( network, &Network::user_registered,   gui, &GUI::user_registered );

    hide_preview();
    gui->show();
}

Client::~Client() {
    delete gui;
    delete network;
    delete settings;
}

Responce *Client::send_text(UserID reciver_id, const QString &text) {
    return network->send_text(reciver_id, text);
}

Responce *Client::sign_up( const QString& login, const QString& pass, const QString& email ) {
    return network->sign_up(login, pass, email);
}

Responce *Client::sign_in(const QString &login, const QString &pass) {
    return network->sign_in(login, pass);
}

Responce *Client::datetime_of_last_message(UserID user_id) {
    return network->datetime_of_last_message( user_id );
}

Responce *Client::last_messages(UserID user_id, const QDateTime &datetime, qint32 max_count) {
    return network->last_messages( user_id, datetime, max_count );
}

Responce *Client::get_users() {
    return network->get_users();
}

bool Client::check_connection() {
    return network->check_connection();
}

void Client::show_preview() {
    if (preview == nullptr) {
        preview_fox_img = new QPixmap(preview_fox_path);
        preview = new QSplashScreen( preview_fox_img->scaled( preview_size ) );
    }
    preview->show();
}

void Client::hide_preview() {
    preview->hide();
}
