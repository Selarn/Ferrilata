#include "storage.h"

Storage::Storage() : QObject(0) {
    dialogs.clear();
    positions.clear();
}

Storage::~Storage() {

}

void Storage::add_message(qint32 sender_id, MessageData *data) {
    dialogs[sender_id].push_back( new MessageData( *data ) );
}

const QVector<Contact *> Storage::get_dialogs() {
    return contacts.values().toVector();
}

const QVector<MessageData *> Storage::first_messages(qint32 user_id, int count) {
    auto iterator = dialogs[user_id].begin();
    auto end_iter = dialogs[user_id].end();
    QVector<MessageData*> res;
    for (int i = 0; i < count && iterator != end_iter; ++i) {
        res.push_back( *iterator );
        ++iterator;
    }
    return res;
}
