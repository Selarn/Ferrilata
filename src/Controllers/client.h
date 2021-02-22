#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QSplashScreen>
#include <QObject>

#include "src/Cryptography/cryptography.h"
#include "src/Settings/settings.h"
#include "src/Network/network.h"
#include "src/UI/gui.h"

class Client : protected QObject {
        Q_OBJECT
        // major interfaces
        GUI* gui;
        Network* network;
        Settings* settings;

    public:
        Client(QString ip, qint16 port);
        ~Client();
        // for GUI
        Responce* send_text(  qint32 reciver_id, const QString& text );
        Responce* sign_up( const QString& login, const QString& pass, const QString& email );
        Responce* sign_in( const QString& login, const QString& pass );
        Responce* datetime_of_last_message( qint32 user_id );
        Responce* last_messages( qint32 user_id, const QDateTime& datetime, qint32 max_count );
        Responce* get_users();
        bool check_connection();

    private:
        // preview
        const QString preview_fox_path = ":/img/1.jpg";
        const QSize preview_size = {600, 400};
        QPixmap* preview_fox_img = nullptr; // don't change
        QSplashScreen* preview = nullptr; // don't change
        void show_preview();
        void hide_preview();

};

#endif // CONTROLLER_H
