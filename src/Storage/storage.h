#ifndef STORAGE_H
#define STORAGE_H

#include "src/Network/responces.h"
#include "src/Storage/contact.h"

#include <QDateTime>
#include <QVector>
#include <QObject>
#include <QMap>

class Storage : protected QObject {
        Q_OBJECT
        QMap<qint32, QVector<MessageData*>> dialogs;
        QMap<qint32, Contact*> contacts;
        QMap<qint32, int> positions; // numbers of viewed messages
    public:
        Storage();
        ~Storage();

        void add_message(qint32 sender_id, MessageData* data);

        const QVector<Contact*> get_dialogs();

        const QVector<MessageData*> first_messages(qint32 user_id, int count);
        const QVector<MessageData*> next_messages (qint32 user_id, int count);
        const QVector<MessageData*> get_messages  (qint32 user_id, int first_number, int count);

    private:
};

#endif // STORAGE_H
