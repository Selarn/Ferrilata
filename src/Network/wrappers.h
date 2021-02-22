#ifndef WRAPPER_H
#define WRAPPER_H

#include <QDateTime>
#include <QVector>
#include <QString>
#include <QPair>

class WRAP_CONTENT {
public:
    static const int NOTHING    = 0,
                     STRING     = 1,
                     DATE_TIME  = 2,
                     UINT_8     = 3,
                     INT_32     = 4,
                     INT_64     = 5,
                     MESS_VECT  = 6,
                     BOOL       = 7,
                     USERS_VECT = 8;
};

class Wrapper {
        int type;
    public:
        int get_type() {return type;}
        Wrapper(int type_): type(type_) {}
        virtual ~Wrapper() {}
};

class StringWrap: public Wrapper {
    public:
        QString data;
        StringWrap( QString val ): Wrapper(WRAP_CONTENT::STRING), data(val) {}
         ~StringWrap() {}
};

class DateTimeWrap: public Wrapper {
    public:
        QDateTime data;
        DateTimeWrap( QDateTime val ): Wrapper(WRAP_CONTENT::DATE_TIME), data(val) {}
        ~DateTimeWrap() {}
};

class UInt8Wrap: public Wrapper {
    public:
        quint8 data;
        UInt8Wrap( quint8 val ): Wrapper(WRAP_CONTENT::UINT_8), data(val) {}
        ~UInt8Wrap() {}
};

class Int32Wrap: public Wrapper {
    public:
        qint32 data;
        Int32Wrap( qint32 val ): Wrapper(WRAP_CONTENT::INT_32), data(val) {}
        ~Int32Wrap() {}
};

class Int64Wrap: public Wrapper {
    public:
        qint64 data;
        Int64Wrap( qint64 val ): Wrapper(WRAP_CONTENT::INT_64), data(val) {}
        ~Int64Wrap() {}
};



struct SSTextMessage {
    qint32 sender_id;
    QString text;
    QDateTime datetime;
};
typedef QVector<SSTextMessage> MessagesVect;

class MessVectWrap: public Wrapper {
    public:
        MessagesVect data;
        MessVectWrap( MessagesVect val ): Wrapper(WRAP_CONTENT::MESS_VECT), data(val) {}
        ~MessVectWrap() {}
};

class BoolWrap: public Wrapper {
    public:
        bool data;
        BoolWrap( bool val ): Wrapper(WRAP_CONTENT::BOOL), data(val) {}
        ~BoolWrap() {}
};

typedef QVector<QPair<qint32,QString>> UsersVect;

class UsersVectWrap: public Wrapper {
    public:
        UsersVect data;
        UsersVectWrap( UsersVect val ): Wrapper(WRAP_CONTENT::USERS_VECT), data(val) {}
        ~UsersVectWrap() {}
};

#endif // WRAPPER_H
