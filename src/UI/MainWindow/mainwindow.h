#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/UI/Widgets/dialogpreview.h"
#include "src/UI/Widgets/dialog.h"
#include "src/typedefs.h"

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QPair>
#include <QMap>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
        Q_OBJECT

    signals:
        void send_text( UserID reciver_id, const QString& text );

    public:
        explicit MainWindow( QWidget* parent = 0 );
        ~MainWindow();

        void add_dialogs( QVector<QPair<UserID,QString>> users );
        void dialog_viewed( UserID user_id );
        void add_message( UserID sender_id, MessageData* message );
        void message_delivered( UserID user_id, const QDateTime& sending_time );

     public slots:
        void dialog_selected( UserID user_id );

    private:
        Ui::MainWindow *ui;
        QVBoxLayout* dialogs_layout;

        QMap<UserID, Dialog*> dialogs;
        UserID current_dialog;

        void add_dialog( UserID user_id, const QString& username );

    protected slots:
        void send_btn_clicked(bool trash);
};

#endif // MAINWINDOW_H
