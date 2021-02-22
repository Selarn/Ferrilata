#ifndef DIALOG_H
#define DIALOG_H

#include "src/Network/responces.h"
#include "src/UI/Widgets/message.h"

#include <QVBoxLayout>
#include <QFrame>

class Dialog : public QFrame {
        Q_OBJECT

        QVBoxLayout* layout;
        QString username;

    public:
        void add_message( MessageData* message );
        QString get_username();

        explicit Dialog(const QString& username_, QWidget *parent = nullptr);
        virtual ~Dialog();
};

#endif // DIALOG_H
