#ifndef GUI_H
#define GUI_H

#include "src/UI/Authentification/authentification.h"
#include "src/UI/MainWindow/mainwindow.h"
#include "src/Storage/storage.h"

#include <QObject>

typedef unsigned int User_ID;

class Client;

class GUI : public QObject {
        Q_OBJECT
        Authentification* auth;
        MainWindow* mainwindow;
        Storage* storage;
        // for crutches
        Client* client;
    public:
        GUI(Client* client);
        ~GUI();
        // for Client
        void show();

    public slots:
        void add_message( qint32 sender_id, MessageData* message );
        void message_delivered( qint32 user_id, QDateTime* sending_time );
        void dialog_viewed( qint32 user_id );
        void user_registered( qint32 user_id, QString* username );

    signals:
        // General
        void connection_lost();
        void connection_restored();
        // for Authentification
        void login_failed();
        void registation_failed( quint8 error_codes );

    protected slots:
        // for Authentification
        void sign_up( const QString& login, const QString& pass, const QString& email );
        void sign_in( const QString& login, const QString& pass );
        // for Dialogs
        void send_text(  qint32 reciver_id, const QString& text );

    private:
        bool connected;
        // General
        void check_connection();
        void process_error( ErrorResponce* error );
        // for Authentification
        void auth_to_main();
};

#endif // GUI_H
