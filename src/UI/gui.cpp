#include "gui.h"

#include "src/Controllers/client.h"

GUI::GUI(Client *client_) : QObject(0) {
    auth = new Authentification();
    storage = new Storage();
    mainwindow = new MainWindow();
    connected = true;
    client = client_;

    connect(auth, &Authentification::sign_up, this, &GUI::sign_up);
    connect(auth, &Authentification::sign_in, this, &GUI::sign_in);
    connect(this, &GUI::login_failed,         auth, &Authentification::show_login_error);
    connect(this, &GUI::registation_failed,   auth, &Authentification::show_registr_error);
    connect(mainwindow, &MainWindow::send_text, this, &GUI::send_text );
}

GUI::~GUI() {
    delete auth;
    delete storage;
    delete mainwindow;
}

void GUI::show() {
    auth->show();
}

void GUI::add_message(qint32 sender_id, MessageData *message) {
    mainwindow->add_message(sender_id, message);
    storage->add_message(sender_id, message);
    delete message;
}

void GUI::message_delivered(qint32 user_id, QDateTime *sending_time) {
    mainwindow->message_delivered( user_id, *sending_time );
    delete sending_time;
}

void GUI::dialog_viewed(qint32 user_id) {
    mainwindow->dialog_viewed( user_id );
}

void GUI::user_registered(qint32 user_id, QString *username) {
    qDebug() << "Get information of registration user \"" << *username << "\" with id " << QString::number(user_id);
    mainwindow->add_dialogs({ {user_id, *username} });
    delete username;
}

void GUI::send_text(qint32 reciver_id, const QString &text) {
    storage->add_message(reciver_id, (MessageData*) new TextMessageData(reciver_id, text, QDateTime::currentDateTime(), false ));
    Responce* res = client->send_text(reciver_id, text);
    if ( res->is_error() ) {
        process_error( (ErrorResponce*)res );
    }
}

void GUI::sign_up(const QString &login, const QString &pass, const QString &email) {
    Responce* resp = client->sign_up(login, pass, email);

    if ( resp->is_error() ) {
        process_error( (ErrorResponce*)resp );
        return;
    }
    if ( resp->get_type() != Responce::UINT_8 ) {
        qDebug() << "Responce with type " << QString::number(resp->get_type()) << " in GUI::sign_up() ";
    }

    quint8 err_codes = ((UInt8Resp*)resp)->res;
    delete resp;

    if ( err_codes ) {
        emit registation_failed( err_codes );
    } else {
        mainwindow->setWindowTitle( login );
        auth_to_main();
    }
}

void GUI::sign_in(const QString &login, const QString &pass) {
    Responce* resp = client->sign_in(login, pass);

    if ( resp->is_error() ) {
        process_error( (ErrorResponce*)resp );
        return;
    }
    if ( resp->get_type() != Responce::BOOL ) {
        qDebug() << "Responce with type " << QString::number(resp->get_type()) << " in GUI::sign_in() ";
    }

    bool resault = ((BoolResp*)resp)->res;
    delete resp;

    if ( resault ) {
        mainwindow->setWindowTitle( login );
        auth_to_main();
    } else {
        emit login_failed();
    }
}

void GUI::check_connection() {
    if ( client->check_connection() ) {
        if ( connected == false ) {
            connected = true;
            emit connection_restored();
        }
    } else
        if ( connected ) {
            connected = false;
            emit connection_lost();
        }
}

void GUI::process_error(ErrorResponce *error) {
    QMessageBox::warning(0, error->get_title(), error->get_text());
    if ( error->get_type() == ErrorResponce::TIME_LIMIT )
        check_connection();
}

void GUI::auth_to_main() {
    auth->hide();

    Responce* res = client->get_users();
    if ( res->is_error() ) {
        process_error( (ErrorResponce*)res );
    }
    else if ( res->get_type() == Responce::USERS_SET ) {
        UsersVectResp* users = (UsersVectResp*)res;
        mainwindow->add_dialogs( users->res );
    }
    else
        qDebug() << "Smth gone wrong!";

    mainwindow->show();
}
