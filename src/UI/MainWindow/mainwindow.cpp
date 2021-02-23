#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow( QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    current_dialog = -1;

  // fill QScrollArea with dialogs
    QFrame* dialogs_frame = new QFrame(this);
    dialogs_layout = new QVBoxLayout(dialogs_frame);

    ui->dialogs->setWidget(dialogs_frame);
    ui->dialogs->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    ui->dialogs->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
  //
    ui->messages->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    ui->messages->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
  //
    connect( ui->send_btn, &QPushButton::clicked, this, &MainWindow::send_btn_clicked );
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::add_dialogs(QVector<QPair<UserID, QString> > users) {
    for ( auto user: users )
        add_dialog( user.first, user.second );
}

void MainWindow::dialog_viewed(UserID user_id) {
    qDebug() << "Dialog with user " << QString::number(user_id) << " viewed.";
}

void MainWindow::add_message(UserID sender_id, MessageData* message) {
    if (dialogs.find(sender_id) == dialogs.end()) {
        // add_dialog( sender_id );
        qDebug() << "Message from unkmown user";
        return;
    }
    dialogs[sender_id]->add_message(message);
}

void MainWindow::message_delivered(UserID user_id, const QDateTime &sending_time) {
    qDebug() << "Message to user " << QString::number(user_id) << " sended " << sending_time.time().toString() << " was delivered.";
}

void MainWindow::dialog_selected(UserID user_id) {
    if ( current_dialog != user_id ) {
        current_dialog = user_id;
        Dialog* dialog = dialogs[user_id];
        ui->username->setText( dialog->get_username() );
        QWidget* Scroll_Bar_Fix = ui->messages->takeWidget();
        ui->messages->setWidget( dialog );
    }
}

void MainWindow::add_dialog(UserID user_id, const QString &username) {
    DialogPreviewCrutch* item = new DialogPreviewCrutch(user_id, username, this);
    connect( item, &DialogPreviewCrutch::dialog_selected, this, &MainWindow::dialog_selected );
    dialogs_layout->addWidget(item);

    Dialog* dialog = new Dialog(username, this);
    dialogs[ user_id ] = dialog;
    if ( current_dialog == -1 ) {
        current_dialog = user_id;
        ui->username->setText( username );
        ui->messages->setWidget( dialog );
    }
}

void MainWindow::send_btn_clicked(bool trash) {
    QString text = ui->lineEdit->text();
    ui->lineEdit->clear();
    emit send_text( current_dialog, text );
}
