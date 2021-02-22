#ifndef NETWORK_H
#define NETWORK_H

#include "src/Network/responces.h"
#include "src/Network/wrappers.h"
#include "src/Network/package.h"

#include <QCryptographicHash>
#include <QCoreApplication>
#include <QHostAddress>
#include <QMessageBox>
#include <QDataStream>
#include <QBasicTimer>
#include <QTcpSocket>
#include <QTime>

#include <cstdarg>
#include <list>


/*
namespace ReqStates {
    namespace CheckConn {
        bool processed;
    }
    namespace  Login {
        Net_list list;
    }
    namespace Registration {
        Net_list list;
    }
    namespace Messages {
        Net_list list;
    }
    namespace Requests {
        Net_list list;
    }
}
*/

//ToDo:
// заменинить namespace ReqStates... на  Net_list requests.
// Возможно в методах sign_up & log_in не проверяется, пришёл ли ответ
// Добавить метод wait_reply( int id )
//     для замены циклов while ( !processed[-1] && Req::Hq::Gq::list.find( -1 ) ) { QCoreApplic.process_process }
// Заменить комментарий с id на консанты. Например const int check_conn_id = 1000;


// (пока не) Поддерживает протокол FoxUp v1.0
class Network: public QObject {
        Q_OBJECT

    signals:

        void message_recived( qint32 sender_id, MessageData* message );
        void message_delivered( qint32 user_id, QDateTime* sending_time );
        void dialog_viewed( qint32 user_id );
        void user_registered( qint32 user_id, QString* username );

    public:

        Responce* sign_up( const QString& login, const QString& pass, const QString& email );
        Responce* sign_in( const QString& login, const QString& pass );
        Responce* send_text(  qint32 reciver_id, const QString& text );
        Responce* datetime_of_last_message( qint32 user_id );
        Responce* last_messages( qint32 user_id, const QDateTime& datetime, qint32 max_count );
        Responce* get_users();

        bool check_connection(); // not finished

        Network( const QHostAddress& address, quint16 port ); // not finished
        ~Network();

    private:
        const int arrays_size = 1003;
        Responce ** responce;
        bool* processed;
        //id:
        // 1-999 - requests
        // 1000 - ack
        // 1001 - login
        // 1002 - registration

        QTcpSocket* socket;
        bool processing;
        qint64 mess_id;

        QString login;
        QString pass;

        void send( qint8 type, qint8 subtype, QVector<Wrapper*> data ); // call "delete" for every Wrapper*

        void next_mess_id();
        void clear_tmp( int id );

        void process_ack    ( Package* package );
        void process_error  ( Package *package );
        void process_request( Package* package );
        void process_reply  ( Package *package );
        void process_notify ( Package* package ); // ToDo

        //void send( qint8 type, qint8 subtype, ... ); // ancient evil
        bool log_in();

    protected slots:
        void process_message(); // not finished
};

// cleared every N msec
class Net_list: public QObject {
        Q_OBJECT

        std::list< std::pair<qint64, bool> > list;
        virtual void timerEvent( QTimerEvent* event );

    public:
        int size();
        void push_back( const qint64& id );
        static void start_timers(int log, int reg, int mess , int reqs);
        bool find( const qint64& id );
        Net_list(int timeout = 0 );
        ~Net_list();
};



//struct for FalseNetwork
struct User {
    QString login;
    QString pass;
    QString mail;
};

class FalseNetwork: public QObject {
        Q_OBJECT

        QVector<User> users;

    signals:

        void message_recived( qint32 sender_id, MessageData* message );

    public:

        Responce* sign_up( const QString& login, const QString& pass, const QString& email );
        Responce* sign_in( const QString& login, const QString& pass );
        Responce* send_text(  qint32 reciver_id, const QString& text );

        bool check_connection();

        FalseNetwork( const QHostAddress& address, quint16 port );
        ~FalseNetwork();
};

#endif // NETWORK_H
