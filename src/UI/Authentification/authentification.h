#ifndef Authentification_H
#define Authentification_H

#include <QPushButton>
#include <QMessageBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QWidget>
#include <QLabel>


namespace Ui {
    class Authentification;
}


class Authentification : public QWidget {
        Q_OBJECT

    public:
        Authentification();
        ~Authentification();

    signals:
        void sign_up( const QString& login, const QString& pass, const QString& email );
        void sign_in( const QString& login, const QString& pass );

    public slots:
        void show_login_error();
        void show_registr_error( int codes );

    protected slots:
        void show_login();
        void show_registr();
        void process_login_form();
        void process_registr_form();

    private:
        Ui::Authentification* ui;
        QString state;
        //available states
        //  "clean"
        //  "login"
        //  "registr"

        QLabel* login_label;
        QLineEdit* login_edit;

        QLabel* mail_label;
        QLineEdit* mail_edit;

        QLabel* pass_label;
        QLineEdit* pass_edit;

        QLabel* pass_ack_label;
        QLineEdit* pass_ack_edit;

        QPushButton* sign_up_btn;
        QFormLayout* reg_layout;

        void set_buttons_block( bool block );

        void set_login_visiable( bool vis );
        void set_registr_visiable( bool vis );

        void show_error( int code );
        //err codes
        //   0 - Incorrect login
        //   1 - Incorrect password
        //   2 - Incorrect email
        //   3 - passwords not equal
        bool fields_check();
        bool check_mail( const QString& mail );
        bool check_pass( const QString& pass );
};


#endif // Authentification_H
