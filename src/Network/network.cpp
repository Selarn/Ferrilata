#include "network.h"


namespace ReqStates {
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


Network::Network(const QHostAddress& address, quint16 port) : QObject() {
    processing = false;
    socket = new QTcpSocket();
    processed = new bool[arrays_size];
    responce = new Responce*[arrays_size];
    for ( int i = 0; i < arrays_size; ++i ) {
        processed[i] = false;
        responce[i] = nullptr;
    }    
    
    Net_list::start_timers( 600'000, 600'000, 600'000, 600'000 ); // 6 fig values only for debug
    
    socket->connectToHost( address, port );
    connect( socket, SIGNAL(readyRead()), this,  SLOT(process_message() ) );
    qDebug() << "Network Ctor finished";
}

void Network::next_mess_id() {
    ++mess_id;
    if ( mess_id % 1000 == 0 )
        mess_id -= 1000;
}

void Network::clear_tmp(int id) {
    if ( id > arrays_size || id < 0 ) {
        qDebug() << "Network::clear_tmp() called with incorrect arg id=" << QString::number(id);
        return;
    }
    if ( responce[id] != nullptr ) {
        //delete responce[id];
        responce[id] = nullptr;
    }
    processed[id] = false;
}

void Network::process_ack(Package *package) {
    if ( package->subtype < 1 || package->subtype > 2 ) {
        qDebug() << "Strange subtype of ack package. Subtype: " << QString::number(package->subtype);
        return;
    }
    if ( package->subtype == 1 ) {
        processed[1000] = true;
    }
    else if ( package->subtype == 2 ) {
        processed[ package->id % 1000 ] = true;
    }
    qDebug() << "ACK package with type 0:" << QString::number(package->subtype) << " was processed";
}

void Network::process_error(Package *package) {
    if ( package->subtype < 0 || package->subtype > 2 ) {
        qDebug() << "Strange subtype of error package. Subtype: " << QString::number(package->subtype);
        return;
    }
    // 13 = sizeof(qint8) + sizeof(qint32) +  sizeof(qint64)
    if ( package->length != 13 + ( package->subtype == 2 ? 1 : 0 ) ) { // Какая-то странная проверка. Не знаю зачем она здесь, пусть будет.
        qDebug() << "Strange length of pack with type 1-" << QString::number(package->subtype) << ". Length: " << QString::number(package->length);
    }
    if ( package->subtype == 0 ) {
        responce[package->id % 1000] = new PermissionDeniedErr("Permission denied");
        processed[package->id % 1000] = true;
    }
    else if ( package->subtype == 1 ) {
        responce[1001] = new BoolResp( false );
        processed[1001] = true;
    }
    else if ( package->subtype == 2 ) {
        quint8 err;
        (*package) >> err;
        if ( err < 1 )
            qDebug() << "Strange error " << QString::number(err) << " in responce for registration";
        responce[1002] = new UInt8Resp( err );
        processed[1002] = true;
    }
    qDebug() << "ERROR package with type 1:" << QString::number(package->subtype) << " was processed";
}

void Network::process_request(Package *package) {
    qDebug() << "Strange package with type 2-" << QString::number(package->subtype) << " requests smth from the client";
}

void Network::process_reply(Package *package) {
    if ( package->subtype < 0 || package->subtype > 3 ) {
        qDebug() << "Strange subtype of reply package. Subtype: " << QString::number(package->subtype);
        return;
    }
    if ( package->subtype == 0 ) {
        (*package) >> mess_id;
        Responce* resp = (Responce*)new TiVoshelResp();
        responce[1001] = resp;
        processed[1001] = true;
        responce[1002] = resp;
        processed[1002] = true;
    }
    else if ( package->subtype == 1 ) {
        QDateTime datetime;
        (*package) >> datetime;
        responce[ package->id % 1000 ] = new DateTimeResp( datetime );
        processed[ package->id % 1000 ] = true;
    }
    else if ( package->subtype == 2 ) {
        MessagesVect res;
        (*package) >> res;
        responce[ package->id % 1000 ] = new TextMessSetResp( res );
        processed[ package->id % 1000 ] = true;
    }
    else if ( package->subtype == 3 ) {
        UsersVect res;
        (*package) >> res;
        responce[ package->id % 1000 ] = new UsersVectResp( res );
        processed[ package->id % 1000 ] = true;
    }
    qDebug() << "Reply package with type 3:" << QString::number(package->subtype) << " was processed";
}

void Network::process_notify(Package *package) {
    if ( package->subtype < 0 || package->subtype > 3 ) {
        qDebug() << "Strange subtype of notify package. Subtype: " << QString::number(package->subtype);
        return;
    }
    if ( package->subtype == 0 ) {
        QDateTime datetime;
        QString text;
        bool recived;
        UserID user_id;
        (*package) >> user_id >> text >> datetime >> recived;
        emit message_recived( user_id, (MessageData*)new TextMessageData(user_id, text, datetime, recived) );
    }
    else if ( package->subtype == 1 ) {
        QDateTime datetime;
        UserID user_id;
        (*package) >> user_id >> datetime;
        emit message_delivered( user_id, new QDateTime(datetime) );
    }
    else if ( package->subtype == 2 ) {
        UserID user_id;
        (*package) >> user_id;
        emit dialog_viewed( user_id );
    }
    else if (package->subtype == 3 ) {
        UserID user_id;
        QString username;
        (*package) >> user_id >> username;
        emit user_registered( user_id, new QString(username) );
    }
    qDebug() << "Notify package with type 3:" << QString::number(package->subtype) << " was processed";
}

bool Network::log_in() {
    static const int login_code = 1001;
    if ( ReqStates::Login::list.size() )
        return false;
    send( 2, 0, {(Wrapper*)new StringWrap(login), (Wrapper*)new StringWrap(pass),} );
    ReqStates::Login::list.push_back( 1 );

    while ( ReqStates::Login::list.size() && !processed[login_code] )
        QCoreApplication::processEvents();

    bool res;
    if ( processed[login_code] == false ) {
        res = false;
    }
    else {
        res = responce[login_code]->get_type() == Responce::YEAH;
    }
    clear_tmp( login_code );
    return res;
}

void Network::process_message() {
    processing = true;

    Package package( socket );

    while ( package.read_header() && package.wait_data() ) {
        switch ( package.type ) {
            case 0: process_ack    ( &package ); break;
            case 1: process_error  ( &package ); break;
            case 2: process_request( &package ); break;
            case 3: process_reply  ( &package ); break;
            case 4: process_notify ( &package ); break;
            default: qDebug() << "Strange request with type " << QString::number(package.type) << ":" << QString::number(package.subtype);
        }
    }
    processing = false;
}

Responce* Network::sign_up(const QString& login, const QString& pass, const QString& email) {
    static const int reg_code = 1002;
    if ( ReqStates::Registration::list.size() ) {
        return new TimeLimitErr("Запрос на регистрацию был отправлен до обработки предыдущего");
    }
    send( 2, 1, {(Wrapper*)new StringWrap(login),
                 (Wrapper*)new StringWrap(pass),
                 (Wrapper*)new StringWrap(email) } );
    ReqStates::Registration::list.push_back( 1 );

    while ( ReqStates::Registration::list.size() && !processed[reg_code] )
        QCoreApplication::processEvents();

    Responce* res;
    if ( processed[reg_code] ) {
        if ( responce[reg_code]->get_type() == Responce::YEAH ) {
            res = new UInt8Resp(0);
        } else
            res = responce[reg_code];
    } else
        res = new TimeLimitErr("Превышено время ожидания запроса");
    clear_tmp( reg_code );
    return res;
}

Responce* Network::sign_in(const QString& login, const QString& pass) {
    this->login = login;
    this->pass = pass;
    return new BoolResp(log_in());
}

Responce* Network::send_text(UserID rec_id, const QString& text) {
    int number = mess_id % 1000;
    send( 2, 4, {(Wrapper*)new Int32Wrap(rec_id),
                 (Wrapper*)new StringWrap(text),
                 (Wrapper*)new DateTimeWrap(QDateTime::currentDateTime()) } );
    ReqStates::Requests::list.push_back( number );

    while ( !processed[number] && ReqStates::Requests::list.find( number ) )
        QCoreApplication::processEvents();

    Responce* res;
    if ( processed[number] ) {
        res = responce[number];
    } else
        res = new TimeLimitErr("Превышено время ожидания запроса");
    clear_tmp( number );
    return res;
}

Responce *Network::datetime_of_last_message(UserID user_id) {
    int number = mess_id % 1000;
    send( 2, 2, {(Wrapper*)new Int32Wrap(user_id)} );
    ReqStates::Requests::list.push_back( number );

    while ( !processed[number] && ReqStates::Requests::list.find( number ) )
        QCoreApplication::processEvents();

    Responce* res;
    if ( processed[number] ) {
        res = responce[number];
    } else
        res = new TimeLimitErr("Превышено время ожидания запроса");
    clear_tmp( number );
    return res;
}

Responce *Network::last_messages(UserID user_id, const QDateTime &datetime, qint32 max_count) {
    int number = mess_id % 1000;
    send( 2, 3, {(Wrapper*)new Int32Wrap(user_id),
                 (Wrapper*)new DateTimeWrap(datetime),
                 (Wrapper*)new Int32Wrap(max_count)} );
    ReqStates::Requests::list.push_back( number );

    while ( !processed[number] && ReqStates::Requests::list.find( number ) )
        QCoreApplication::processEvents();

    Responce* res;
    if ( processed[number] ) {
        res = responce[number];
    } else
        res = new TimeLimitErr("Превышено время ожидания запроса");
    clear_tmp( number );
    return res;
}

Responce *Network::get_users() {
    int number = mess_id % 1000;
    send( 2, 5, {} );
    ReqStates::Requests::list.push_back( number );

    while ( !processed[number] && ReqStates::Requests::list.find( number ) )
        QCoreApplication::processEvents();

    Responce* res;
    if ( processed[number] ) {
        res = responce[number];
    } else
        res = new TimeLimitErr("Превышено время ожидания запроса");
    clear_tmp( number );
    return res;
}

bool Network::check_connection() {
    send( 0, 0, {} );
    ReqStates::Requests::list.push_back( 1000 );
    while ( !processed[1000] && ReqStates::Requests::list.find( 1000 ) )
        QCoreApplication::processEvents();
    return processed[1000];
}

Network::~Network() {
    delete socket;
    delete [] processed;
    for ( int i = 0; i < arrays_size; ++i )
        delete responce[i];
    delete [] responce;
}

void Network::send(qint8 type, qint8 subtype, QVector<Wrapper*> data) {
    qint64 message_id = mess_id;
    next_mess_id();

    QByteArray array;
    QDataStream out( &array, QIODevice::WriteOnly );
    out.setVersion( QDataStream::Qt_5_12 );

    out << qint8(type*16 + subtype) << qint32(0) << message_id; // 13 bytes, 13 = 1 + 4 + 8

    for (Wrapper* xz: data) { // XZ what is 'xz'
        int xz_type = xz->get_type();
        if ( xz_type == WRAP_CONTENT::INT_32 ) {
            Int32Wrap* num_wrap = (Int32Wrap*)xz;
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
            MessagesVect* mes_wrap = (MessagesVect*)xz;
            qint32 count = mes_wrap->size();
            out << count;
            for ( int i = 0; i < count; ++i )
                out << (*mes_wrap)[i].sender_id << (*mes_wrap)[i].text << (*mes_wrap)[i].datetime;
        }
        else if ( xz_type == WRAP_CONTENT::BOOL ) {
            BoolWrap* bool_wrap = (BoolWrap*)xz;
            out << bool_wrap->data;
        }
        else {
            qDebug() << "Strange Wrapper with type " << QString(xz_type) << " in Network::send()";
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



bool check_for_update_list_func( const std::pair<qint64, bool>& item ) {
    if ( item.second )
        qDebug() << "Packge with id " << QString::number(item.first) << " will be deleted";
    return item.second;
}

void Net_list::timerEvent(QTimerEvent* event) {
    list.remove_if( check_for_update_list_func );
    for ( auto it = list.begin(); it != list.end(); ++it )
        it->second = true;
}

int Net_list::size() {
    return (int)list.size();
}

void Net_list::push_back(const qint64 &id) {
    list.push_back( std::pair<qint64, bool> (id, false) );
}

void Net_list::start_timers(int log, int reg, int mess, int reqs) {
    ReqStates::Login::list.startTimer( log );
    ReqStates::Messages::list.startTimer( mess );
    ReqStates::Registration::list.startTimer( reg );
    ReqStates::Requests::list.startTimer( reqs );
}

bool Net_list::find(const qint64 &id) {
    for ( auto it = list.begin(); it != list.end(); ++it )
        if ( it->first == id )
            return true;
    return false;
}

Net_list::Net_list(int timeout): QObject() {
    if ( timeout )
        startTimer( timeout );
}

Net_list::~Net_list() { }

Responce *FalseNetwork::sign_up(const QString &login, const QString &pass, const QString &email) {
    User user = {login, pass, email};
    users.push_back(user);
    return new UInt8Resp(0);
}

Responce *FalseNetwork::sign_in(const QString &login, const QString &pass) {
    bool res = false;
    for ( const User& user: users )
        if ( user.login == login && user.pass == pass ) {
            res = true;
            break;
        }
    return new BoolResp(res);
}

bool FalseNetwork::check_connection() {
    return true;
}

FalseNetwork::FalseNetwork(const QHostAddress &address, quint16 port): QObject() {
    users.push_back({"admin", "admin", "admin"});
    qDebug() << "Network not implemented !!!";
    qDebug() << "Created FalseNetwork with addres " << address.toString() << " and port " << QString::number(port);
}

FalseNetwork::~FalseNetwork() { }

Responce *FalseNetwork::send_text(UserID reciver_id, const QString &text) {
    qDebug() << "<" << text << "> Sended to " << QString::number(reciver_id);
    return new BoolResp(true);
}
