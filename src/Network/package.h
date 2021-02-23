#ifndef PACKAGE_H
#define PACKAGE_H

#include "src/Network/wrappers.h"
#include "src/typedefs.h"

#include <QDataStream>
#include <QDateTime>
#include <QTcpSocket>
#include <QtCore>

// (пока не) Поддерживает протокол FoxUp v1.0
class Package {
    QTcpSocket* sock;
    QDataStream inp;
    QTime time;

    bool wait_data( const qint64 &data_size, int wait_msec = 10000 );

public:

    bool read_header();
    bool wait_data();

    Package& operator >> ( bool& var );
    Package& operator >> ( quint8& var );
    Package& operator >> ( qint32& var );
    Package& operator >> ( qint64& var );
    Package& operator >> ( QString& var );
    Package& operator >> ( QDateTime& var );
    Package& operator >> ( MessagesVect& var );
    Package& operator >> ( UsersVect& var );

    qint8 type;
    qint8 subtype;
    qint32 length;
    qint64 id;

    Package( QTcpSocket* socket );
};

#endif // PACKAGE_H
