#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/UI/Widgets/dialogpreview.h"
#include "src/UI/Widgets/dialog.h"

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
        void send_text(  qint32 reciver_id, const QString& text );

    public:
        explicit MainWindow( QWidget* parent = 0 );
        ~MainWindow();

        void add_dialogs( QVector<QPair<qint32,QString>> users );
        void dialog_viewed( qint32 user_id );
        void add_message( qint32 sender_id, MessageData* message );
        void message_delivered( qint32 user_id, const QDateTime& sending_time );

     public slots:
        void dialog_selected( qint32 user_id );

    private:
        Ui::MainWindow *ui;
        QVBoxLayout* dialogs_layout;

        QMap<qint32, Dialog*> dialogs;
        qint32 current_dialog;

        void add_dialog(qint32 user_id, const QString& username );

    protected slots:
        void send_btn_clicked(bool trash);
};

#endif // MAINWINDOW_H
