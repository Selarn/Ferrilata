#include "server.h"

Server::Server(int port) : QObject(0) {
    storage = new ServerStorage();
    server = new QTcpServer(this);
    users.clear();

    if ( !server->listen(QHostAddress::Any, port) ) {
        QMessageBox::critical(0, "Server error", "unable to start the server:" + server->errorString() );
        server->close();
        return;
    }

    connect( server, &QTcpServer::newConnection, this, &Server::new_connection );
}

void Server::send_text(qint32 sender_id, qint32 reciver_id, const QString &text, const QDateTime &datetime) {
    users[sender_id]->send_text( reciver_id, text, datetime, false );
    storage->save_text_message( sender_id, reciver_id, text, datetime );
    auto reciver = users.find( reciver_id );
    if ( reciver != users.end() )
        (reciver.value())->send_text( sender_id, text, datetime, true );
}

void Server::new_connection() {
    QTcpSocket* client = server->nextPendingConnection();
    connect( client, &QTcpSocket::disconnected, client, &QTcpSocket::deleteLater );
    connect( client, &QTcpSocket::readyRead, this, &Server::read_data );
}

void Server::read_data() {
    QTcpSocket* sock = (QTcpSocket*)sender();
    Package package( sock );

    while ( package.read_header() && package.wait_data() ) {

        if ( package.type == 0 && package.subtype == 0 ) {
            send( 0, 1, sock, package.id );
        }
        else if ( package.type == 2 && package.subtype == 0 ) {
            QString login, pass;
            package >> login >> pass;
            qint32 user_id = storage->log_in( login, pass );
            if ( user_id == 0 ) {
                send( 1, 1, sock, package.id );
            } else {
                add_session( user_id, sock );
                send( 3, 0, sock, package.id, { (Wrapper*)new Int64Wrap( user_id * qint64(1000) ) } );
            }
        }
        else if ( package.type == 2 && package.subtype == 1 ) {
            QString login, pass, mail;
            package >> login >> pass >> mail;
            quint8 errors = storage->sign_up( login, pass, mail );
            if ( errors != 0 ) {
                send( 1, 2, sock, package.id, { (Wrapper*)new UInt8Wrap(errors) } );
            } else {
                qint32 user_id = storage->log_in( login, pass );
                for ( SUser* user: users.values() ) {
                    user->welcome_new_user( user_id, login );
                }
                add_session( user_id, sock );
                send( 3, 0, sock, package.id, { (Wrapper*)new Int64Wrap(user_id* 1000) } );
            }
        }
        else {
            qDebug() << "Strange anonymous request: " << QString::number(package.type) << "," << QString(package.subtype);
            send( 1, 0, sock, package.id );
        }
    }
}

void Server::send(qint8 type, qint8 subtype, QTcpSocket* socket, qint64 mess_id, QVector<Wrapper *> data) {
    QByteArray array;
    QDataStream out( &array, QIODevice::WriteOnly );
    out.setVersion( QDataStream::Qt_5_12 );

    out << qint8(type*16 + subtype) << qint32(0) << mess_id; // 13 bytes, 13 = 1 + 4 + 8

    for (Wrapper* xz: data) { // XZ what is 'xz'
        int xz_type = xz->get_type();
        if ( xz_type == WRAP_CONTENT::INT_32 ) {
            Int32Wrap* num_wrap = (Int32Wrap*)xz;
            out << num_wrap->data;
        }
        else if ( xz_type == WRAP_CONTENT::INT_64 ) {
            Int64Wrap* num_wrap = (Int64Wrap*)xz;
            out << num_wrap->data;
        }
        else if ( xz_type == WRAP_CONTENT::STRING ) {
            StringWrap* str_wrap = (StringWrap*)xz;
            out << str_wrap->data;
        }
        else if ( xz_type == WRAP_CONTENT::DATE_TIME ) {
            DateTimeWrap* dat_wrap = (DateTimeWrap*)xz;
            out << dat_wrap->data;
        }
        else if ( xz_type == WRAP_CONTENT::MESS_VECT ) {
            MessVectWrap* mes_wrap = (MessVectWrap*)xz;
            qint32 count = mes_wrap->data.size();
            out << count;
            for ( int i = 0; i < count; ++i )
                out << mes_wrap->data[i].sender_id << mes_wrap->data[i].text << mes_wrap->data[i].datetime;
        }
        else if ( xz_type == WRAP_CONTENT::BOOL ) {
            BoolWrap* bool_wrap = (BoolWrap*)xz;
            out << bool_wrap->data;
        }
        else if ( xz_type == WRAP_CONTENT::USERS_VECT ) {
            UsersVectWrap* users_wrap = (UsersVectWrap*)xz;
            out << (qint32)(users_wrap->data.size());
            for ( auto user: users_wrap->data )
                out << user.first << user.second;
        }
        else {
            qDebug() << "Strange Wrapper with type " << QString::number(xz_type) << " in Server::send()";
        }
        delete xz;
    }

    if ( out.status() != QDataStream::Ok ) {
        qDebug() << "Failed to write data to bytearray (???)";
        return;
    }

    out.device()->seek(1);
    out << qint32(array.size());

    socket->write( array );
}

SUser* Server::get_user(qint32 user_id) {
    return users[user_id];
}

ServerStorage* Server::get_storage() {
    return storage;
}

void Server::delete_user_session(qint32 user_id) {
    if ( users.find( user_id ) != users.end() ) {
        users[user_id]->deleteLater();
        users.remove( user_id );
    }
}

void Server::add_session(qint32 user_id, QTcpSocket *socket) {
    disconnect( socket, &QTcpSocket::readyRead, this, &Server::read_data );
    if ( users.find( user_id ) == users.end() ) {
        SUser* user = new SUser(user_id, this);
        connect( user, &SUser::delete_user_session, this, &Server::delete_user_session );
        users[user_id] = user;
    }
    users[user_id]->add_session( socket );
}


//_User_________________________________________________________________________________________________________________________________________________________________/\User|

SUser::SUser(qint32 user_id, Server *server_): QObject(server_) {
    channels.clear();
    server = server_;
    id = user_id;
}

void SUser::add_session(QTcpSocket *socket) {
    connect( socket, &QTcpSocket::disconnected, this, &SUser::delete_channel );
    Channel* channel = new Channel( socket, id, server );
    channels.push_back( channel );
}

void SUser::send_text(qint32 user_id, const QString &text, const QDateTime &datetime, bool recived) {
    for ( Channel* channel: channels ) {
        Server::send( 4, 0, channel->get_socket(), 0, { (Wrapper*)new Int32Wrap(user_id),
                                                        (Wrapper*)new StringWrap(text),
                                                        (Wrapper*)new DateTimeWrap(datetime),
                                                        (Wrapper*)new BoolWrap(recived)       } );
    }
}

void SUser::welcome_new_user(qint32 user_id, const QString &username) {
    for ( Channel* channel: channels ) {
        Server::send( 4, 3, channel->get_socket(), 0, {(Wrapper*)new Int32Wrap(user_id), (Wrapper*)new StringWrap(username)} );
        send_text( user_id, "Hey there! I am using Ferrilata messanger!", QDateTime::currentDateTime(), true );
    }
}

void SUser::delete_channel() {
    QTcpSocket* disc_socket = (QTcpSocket*)sender();
    for ( Channel* channel: channels ) {
        if ( channel->get_socket() == disc_socket )
            channel->deleteLater();
    }
    if ( channels.empty() )
        emit delete_user_session( id );
}



//_Channel___________________________________________________________________________________________________________________________________________________________/\Channel|

Channel::Channel(QTcpSocket *socket_, qint32 user_id_, Server *server_): QObject(server_) {
    curr_user_id = user_id_;
    socket = socket_;
    server = server_;
    processing = false;

    connect( socket, &QTcpSocket::readyRead, this, &Channel::process_data );
}

QTcpSocket *Channel::get_socket() {
    return socket;
}

void Channel::process_data() {
    if ( processing )
        return;
    processing = true;

    Package package( socket );
    while ( package.read_header() && package.wait_data() ) {
        if ( package.type == 0 && package.subtype == 0) {
            Server::send( 0, 1, socket, package.id );
        }
        else if ( package.type == 2 ) {
            if ( package.subtype == 2 ) {
                qint32 user_id;
                package >> user_id;
                QDateTime last_datetime = server->get_storage()->last_datetime( curr_user_id, user_id );
                Server::send( 3, 1, socket, package.id, { (Wrapper*)new DateTimeWrap(last_datetime) } );
            }
            else if ( package.subtype == 3 ) {
                qint32 user_id, max_count;
                QDateTime datetime;
                package >> user_id >> datetime >> max_count;
                if ( max_count > 100 ) {
                    Server::send( 4, 0, socket, 0, { (Wrapper*)new Int32Wrap(0), (Wrapper*)new StringWrap("Nice DDOS."), (Wrapper*)new DateTimeWrap(QDateTime::currentDateTime()) } );
                    max_count %= 100;
                }
                MessagesVect messages = server->get_storage()->last_messages( curr_user_id, user_id, datetime, max_count );
                Server::send( 3, 2, socket, package.id, { (Wrapper*)new MessVectWrap(messages) } );
            }
            else if ( package.subtype == 4 ) {
                QDateTime datetime;
                qint32 reciver_id;
                QString text;
                package >> reciver_id >> text >> datetime;
                Server::send( 0, 2, socket, package.id );
                server->send_text( curr_user_id, reciver_id, text, datetime );                
            }
            else if ( package.subtype == 5 ) {
                Server::send( 3, 3, socket, package.id, { (Wrapper*)new UsersVectWrap(server->get_storage()->get_users()) });
            }
            else {
                qDebug() << "Strange requst with subtype: " << QString::number(package.subtype);
            }
        }
        else {
            qDebug() << "Strange package with type: " << QString::number(package.type) << " and subtype: " << QString::number(package.subtype);
        }
    }
    processing = false;
}

//_Storage___________________________________________________________________________________________________________________________________________________________/\Storage|

qint64 ServerStorage::dialog_id(qint32 first_user_id, qint32 sec_user_id) {
    qint64 max, min;
    if ( first_user_id > sec_user_id ) {
        max = first_user_id;
        min = sec_user_id;
    } else {
        max = sec_user_id;
        min = first_user_id;
    }
    return min * (qint64(1)<<32) + max; // min * 2^32 + max
}

bool ServerStorage::pass_check(const QString &pass) {
    return pass.size() > 6;
}

void ServerStorage::add_user(const QString &login, const QString &pass, const QString &mail) {
    qint32 user_id = new_id();
    SSUser* user = new SSUser({user_id, login, pass, mail});
    users[user_id] = user;
    users_by_login[login] = user;
}

qint32 ServerStorage::new_id() {
    static qint32 min_unused_id = 0;
    return min_unused_id++;
}

ServerStorage::ServerStorage() {
    users.clear();
    messages.clear();

    add_user( "Ferrilata", "", "MMM@mmm.com" );
    add_user( "Admin", "AdminPass123", "admin@admin.com" );
}

ServerStorage::~ServerStorage() {
    for ( SSUser* user: users.values() )
        delete user;
    for ( auto dialog: messages.values() )
        for ( SSTextMessage* message: dialog )
            delete message;
}

UsersVect ServerStorage::get_users() {
    UsersVect res;
    for ( SSUser* user: users.values() )
        res.push_back({ user->id, user->login });
    return res;
}

const QDateTime &ServerStorage::last_datetime(qint32 first_id, qint32 second_id) {
    qint64 d_id = dialog_id( first_id, second_id );
    return messages[d_id].last()->datetime;
}

MessagesVect ServerStorage::last_messages(qint32 first_id, qint32 second_id, const QDateTime &datetime, qint32 max_count) {
    qint64 d_id = dialog_id( first_id, second_id );
    auto iter = messages[d_id].rbegin();
    MessagesVect res;
    while ( (*iter)->datetime >= datetime && res.size() <= max_count ) {
        res.push_back( **iter );
        ++iter;
    }
    return res;
}

void ServerStorage::save_text_message(qint32 sender_id, qint32 reciver_id, const QString &text, const QDateTime &datetime) {
    qint64 d_id = dialog_id(sender_id, reciver_id);
    messages[d_id].push_back( new SSTextMessage({sender_id, text, datetime}) );
}

qint32 ServerStorage::log_in(const QString &login, const QString &pass) {
    auto user = users_by_login.find( login );
    if ( user == users_by_login.end() )
        return 0;
    return ( user.value()->pass == pass ? user.value()->id : 0 );
}

quint8 ServerStorage::sign_up(const QString &login, const QString &pass, const QString &mail) {
    qint8 errors = 0;
    for ( SSUser* user : users.values() ) {
        if ( user->login == login ) {
            errors += 1;
            break;
        }
        if ( user->mail == mail ) {
            errors += 2;
            break;
        }
    }
    if ( pass_check( pass ) == false )
        errors += 4;
    if ( mail.contains("@rosgvard.ru") )
        errors += 8;

    if ( errors == 0 ) {
        qint32 user_id = new_id();
        SSUser* user = new SSUser({user_id, login, pass, mail});
        users[user_id] = user;
        users_by_login[login] = user;
    }
    return errors;
}
