#ifndef SERVER_H
#define SERVER_H

#include "src/Network/package.h"
#include "src/Network/wrappers.h"

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
        QMap<qint32, SUser*> users; // READ ONLY !!!
        ServerStorage* storage; // READ ONLY !!!

        explicit Server( int port );

        void send_text( qint32 sender_id, qint32 reciver_id, const QString& text, const QDateTime& datetime );

        static void send(qint8 type, qint8 subtype, QTcpSocket *socket, qint64 mess_id = 0, QVector<Wrapper*> data = {} ); // call "delete" for every Wrapper*

    public slots:
        void delete_user_session(qint32 user_id);
        void new_connection();
        void read_data();

    private:
        QTcpServer* server;

        void add_session( qint32 user_id, QTcpSocket* socket );
};



class SUser: public QObject {
        Q_OBJECT

        QVector<Channel*> channels;
        Server* server;
        qint32 id;

    public:
        SUser( qint32 user_id, Server* server_ );

        void add_session( QTcpSocket* socket );
        void send_text( qint32 sender_id, const QString& text, const QDateTime& datetime, bool recived );
        void welcome_new_user( qint32 user_id, const QString& username );

    public slots:
        void delete_channel();

    signals:
        void delete_user_session(qint32 user_id);
};

// Channel is useless trash
class Channel: public QObject {
        Q_OBJECT

        QTcpSocket* socket;
        Server* server;
        qint32 curr_user_id;
        bool processing;

    public:
        Channel(QTcpSocket* socket_, qint32 user_id_, Server* server_ );
        QTcpSocket* get_socket();

    public slots:
        void process_data();
};

// for storage
struct SSUser {
    qint32 id;
    QString login;
    QString pass;
    QString mail;
};

class ServerStorage: public QObject {
        Q_OBJECT

        QMap<qint32, SSUser*> users;
        QMap<QString, SSUser*> users_by_login;
        QMap<qint64, QVector<SSTextMessage*>> messages; // key = min_user_id * 2^32 + max_user_id

    public:        
        ServerStorage();
        ~ServerStorage();

        UsersVect get_users();

        const QDateTime& last_datetime( qint32 first_id, qint32 second_id );
        MessagesVect last_messages( qint32 first_id, qint32 second_id, const QDateTime& datetime, qint32 max_count );
        void save_text_message( qint32 sender_id, qint32 reciver_id, const QString& text, const QDateTime& datetime );

        qint32 log_in( const QString& login, const QString& pass ); // return user id or 0 if fails
        quint8 sign_up( const QString& login, const QString& pass, const QString& mail ); // return something

    private:
        qint64 dialog_id( qint32 first_user_id, qint32 sec_user_id );
        bool pass_check( const QString& pass );
        void add_user( const QString& login, const QString& pass, const QString& mail );
        qint32 new_id();
};
#endif // SERVER_H
