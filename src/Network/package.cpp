#include "package.h"

#include "src/Network/responces.h"

bool Package::wait_data(const qint64& data_size, int wait_msec) {
    time.start();
    while ( sock->bytesAvailable() != data_size && time.elapsed() < wait_msec ) {
        QCoreApplication::processEvents();
    }
    if (sock->bytesAvailable() < data_size) {
        //qDebug() << "Waiting for packeges data was failed, waiting " << QString::number(data_size) << " bytes.";
        return false;
    }
    else
        return true;
}


bool Package::read_header() {
    if ( !wait_data( 13, 5000 ) ) { // 13 bytes = 1 + 4 + 8
        return false;
    }
    inp >> type >> length >> id;
    subtype = type % 16;
    type /= 16;
    return true;
}

bool Package::wait_data() {
    return  wait_data( length - 13 ); // 13 bytes = 1 + 4 + 8
}

Package &Package::operator >>(bool &var) {
    if ( sock->bytesAvailable() < (qint64)sizeof(bool) ) {
        qDebug() << "failed to read bool from package!";
        return *this;
    }
    inp >> var;
    return *this;
}


Package &Package::operator >>(quint8 &var) {
    if ( sock->bytesAvailable() < (qint64)sizeof(qint8) ) {
        qDebug() << "failed to read quint8 from package!";
        return *this;
    }
    inp >> var;
    return *this;
}


Package &Package::operator >>(qint32 &var) {
    if ( sock->bytesAvailable() < (qint64)sizeof(qint32) ) {
        qDebug() << "failed to read qint32 from package!";
        return *this;
    }
    inp >> var;
    return *this;
}

Package &Package::operator >>(qint64 &var) {
    if ( sock->bytesAvailable() < (qint64)sizeof(qint64) ) {
        qDebug() << "failed to read qint64 from package!";
        return *this;
    }
    inp >> var;
    return *this;
}

Package &Package::operator >>(QString &var) {
    if ( sock->bytesAvailable() < (qint64)sizeof(QString) ) {
        qDebug() << "failed to read QString from package!";
        return *this;
    }
    inp >> var;
    return *this;
}

Package &Package::operator >>(QDateTime &var) {
    if ( sock->bytesAvailable() < (qint64)sizeof(QDateTime) ) {
        qDebug() << "failed to read QDateTime from package!";
        return *this;
    }
    inp >> var;
    return *this;
}

Package &Package::operator >>(MessagesVect &var) {
    QDateTime datetime;
    QString text;
    qint32 count, sender_id;
    if ( sock->bytesAvailable() < (qint64)sizeof(qint32) ) {
        qDebug() << "failed to read size of MessagesVect from package!";
        return *this;
    }
    inp >> count;
    if ( sock->bytesAvailable() < count * ((qint64)sizeof(qint32) + (qint64)sizeof(QString) + (qint64)sizeof(QDateTime)) ) {
        qDebug() << "failed to read MessagesVect from package!";
        return *this;
    }
    for ( qint32 i = 0; i < count; ++i ) {
        inp >> sender_id >> text >> datetime;
        var.push_back({ sender_id, text, datetime });
    }
    return *this;
}

Package &Package::operator >>(UsersVect &var) {
    qint32 count, user_id;
    QString username;
    if ( sock->bytesAvailable() < (qint64)sizeof(qint32) ) {
        qDebug() << "failed to read size of MessagesVect from package!";
        return *this;
    }
    inp >> count;
    if ( sock->bytesAvailable() < count * ((qint64)sizeof(qint32) + (qint64)sizeof(QString)) ) {
        qDebug() << "failed to read MessagesVect from package!";
        return *this;
    }
    for ( qint32 i = 0; i < count; ++i ) {
        inp >> user_id >> username;
        var.push_back({ user_id, username });
    }
    return *this;
}


Package::Package(QTcpSocket *socket): inp(socket) {
    inp.setVersion( QDataStream::Qt_5_12 );
    sock = socket;
}

