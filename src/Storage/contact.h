#ifndef CONTACT_H
#define CONTACT_H

#include <QtGlobal>
#include <QString>

class Contact {
    qint32 id;
    QString username;
public:
    qint32 get_id() {return id;}
    QString get_username() {return username;}
    Contact(qint32 id_, QString username_): id(id_), username(username_) {}
};

#endif // CONTACT_H
