#ifndef RESPONCES_H
#define RESPONCES_H

#include "src/Network/wrappers.h"
#include "src/Network/package.h"

#include <QtGlobal>

class Responce {
    protected:
        char type;
        bool error;
    public:
        char get_type();
        bool is_error();
        static const char EMPTY    = 0;
        static const char BOOL     = 1;
        static const char UINT_8   = 2;
        static const char INT_32   = 3;
        static const char DATETIME = 4;
        static const char DELETED_MESSAGE = 5;
        static const char TEXT_MESSAGE    = 6;
        static const char MESSAGES = 7;
        static const char YEAH     = 8;
        static const char TEXT_MESS_SET   = 9;
        static const char USERS_SET   = 10;
        Responce( char type = Responce::EMPTY );
        virtual ~Responce();
};

// Responces
class BoolResp: public Responce {
    public:
        bool res;
        BoolResp( bool val ): Responce(Responce::BOOL), res(val) {}
        ~BoolResp() {}
};


class UInt8Resp: public Responce {
    public:
        quint8 res;
        UInt8Resp( quint8 val ): Responce(Responce::UINT_8), res(val) {}
        ~UInt8Resp() {}
};

class Int32Resp: public Responce {
    public:
        qint32 res;
        Int32Resp( qint32 val ): Responce(Responce::INT_32), res(val) {}
        ~Int32Resp() {}
};

class DateTimeResp: public Responce {
    public:
        QDateTime res;
        DateTimeResp( const QDateTime& val ): Responce(Responce::DATETIME), res(val) {}
};

class TiVoshelResp: public Responce { // WTF ????
    public:
    TiVoshelResp(): Responce(Responce::YEAH) {}
};

class TextMessSetResp: public Responce {
    public:
        MessagesVect res;
        TextMessSetResp( const MessagesVect& val ): Responce(Responce::TEXT_MESS_SET), res(val) {}
};

class UsersVectResp: public Responce {
    public:
        UsersVect res;
        UsersVectResp( const UsersVect& val ): Responce(Responce::USERS_SET), res(val) {}
};


// Messages
class MessageData: public Responce {
        QDateTime datetime;
        qint32 sender_id;
        bool received; // True if received from another user, false if sent by the current user
        char state;
    public:
        MessageData( qint32            sender_id_,
                     const QDateTime&  datetime_,
                     bool              received_,
                     char              type_ = 0):
            Responce(type_), datetime(datetime_), sender_id(sender_id_), received(received_) { state = MessageData::WAIT_UPLOAD; }
        ~MessageData() {}
        void delete_message() { type = Responce::DELETED_MESSAGE; }
        qint32           get_sender_id() { return sender_id; }
        const QDateTime& get_datetime()  { return datetime; }
        char             get_state()     { return state; }
        bool             was_received()  { return received; }
        // States
        static const char WAIT_UPLOAD = 0;
        static const char UPLOADED    = 1;
        static const char DELIVERED   = 2;
        static const char READED      = 3;
};

class TextMessageData: public MessageData {
        QString text;
    public:
        TextMessageData( qint32            sender_id_,
                         const QString&    text_,
                         const QDateTime&  datetime_,
                         bool              received_ ):
            MessageData(sender_id_, datetime_, received_, Responce::TEXT_MESSAGE), text(text_) {}
        ~TextMessageData() {}
        const QString& get_text() { return text; }
};


// Errors
class ErrorResponce: public Responce {
    protected:
        QString title;
        QString text;
    public:
        const QString& get_title();
        const QString& get_text();
        static const char UNKNOWN = 0;
        static const char TIME_LIMIT = 1;
        static const char PERM_DENIED = 2;
        ErrorResponce( char type = ErrorResponce::UNKNOWN );
        ~ErrorResponce();
};

class TimeLimitErr: public ErrorResponce {
    public:
        TimeLimitErr( const QString& error_text );
        ~TimeLimitErr();
};

class PermissionDeniedErr: public ErrorResponce {
    public:
        PermissionDeniedErr( const QString& error_text );
        ~PermissionDeniedErr();
};

#endif // RESPONCES_H
