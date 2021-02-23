#ifndef STORAGE_H
#define STORAGE_H

#include "src/Network/responces.h"
#include "src/Storage/contact.h"
#include "src/typedefs.h"

#include <QDateTime>
#include <QVector>
#include <QObject>
#include <QMap>

class Storage : protected QObject {
        Q_OBJECT
        QMap<UserID, QVector<MessageData*>> dialogs;
        QMap<UserID, Contact*> contacts;
        QMap<UserID, int> positions; // numbers of viewed messages
    public:
        Storage();
        ~Storage();

        void add_message(UserID sender_id, MessageData* data);

        const QVector<Contact*> get_dialogs();

        const QVector<MessageData*> first_messages(UserID user_id, int count);
        const QVector<MessageData*> next_messages (UserID user_id, int count);
        const QVector<MessageData*> get_messages  (UserID user_id, int first_number, int count);

    private:
};

#endif // STORAGE_H
