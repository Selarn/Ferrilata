#ifndef SERVER_H
#define SERVER_H

#include "src/Network/wrappers.h"
#include "src/Network/package.h"
#include "src/typedefs.h"

#include <QMessageBox>
#include <QTcpSocket>
#include <QTcpServer>
#include <QVector>
#include <QObject>
#include <QPair>
#include <QMap>


class SUser;
class Channel;
class ServerStorage;


class Server: public QObject {
        Q_OBJECT

    public:

        explicit Server( int port );

        void send_text( UserID sender_id, UserID reciver_id, const QString& text, const QDateTime& datetime );

        static void send( qint8 type, qint8 subtype, QTcpSocket *socket, qint64 mess_id = 0, QVector<Wrapper*> data = {} ); // Warning: this func call "delete" for every object in "data"

        SUser* get_user( UserID user_id );
        ServerStorage* get_storage();

    public slots:
        void delete_user_session(UserID user_id);
        void new_connection();
        void read_data();

    private:
        QTcpServer* server;
        QMap<UserID, SUser*> users;
        ServerStorage* storage;

        void add_session( UserID user_id, QTcpSocket* socket );
};



class SUser: public QObject {
        Q_OBJECT

        QVector<Channel*> channels;
        Server* server;
        UserID id;

    public:
        SUser( UserID user_id, Server* server_ );

        void add_session( QTcpSocket* socket );
        void send_text( UserID sender_id, const QString& text, const QDateTime& datetime, bool recived );
        void welcome_new_user( UserID user_id, const QString& username );

    public slots:
        void delete_channel();

    signals:
        void delete_user_session( UserID user_id );
};

// Channel is useless trash
class Channel: public QObject {
        Q_OBJECT

        QTcpSocket* socket;
        Server* server;
        UserID curr_user_id;
        bool processing;

    public:
        Channel(QTcpSocket* socket_, UserID user_id_, Server* server_ );
        QTcpSocket* get_socket();

    public slots:
        void process_data();
};

// for storage
struct SSUser {
    UserID id;
    QString login;
    QString pass;
    QString mail;
};

class ServerStorage: public QObject {
        Q_OBJECT

        QMap<UserID, SSUser*> users;
        QMap<QString, SSUser*> users_by_login;
        QMap<qint64, QVector<SSTextMessage*>> messages; // key = lower_user_id * 2^32 + higher_user_id. (I know that it only works if UserID == int32_t)

    public:        
        ServerStorage();
        ~ServerStorage();

        UsersVect get_users();

        const QDateTime& last_datetime( UserID first_id, UserID second_id );
        MessagesVect last_messages( UserID first_id, UserID second_id, const QDateTime& datetime, qint32 max_count );
        void save_text_message( UserID sender_id, UserID reciver_id, const QString& text, const QDateTime& datetime );

        UserID log_in( const QString& login, const QString& pass ); // return user id or 0 if fails
        quint8 sign_up( const QString& login, const QString& pass, const QString& mail ); // return something

    private:
        qint64 dialog_id( UserID first_user_id, UserID sec_user_id );
        bool pass_check( const QString& pass );
        void add_user( const QString& login, const QString& pass, const QString& mail );
        UserID new_id();
};
#endif // SERVER_H
