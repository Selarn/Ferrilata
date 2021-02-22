#include "authentification.h"
#include "ui_authentification.h"

#include <QDebug>

Authentification::Authentification(): QWidget(0), ui(new Ui::Authentification) {
    ui->setupUi(this);
    hide();
    setWindowTitle("Authentification");
    state = "login";
    // add elements for registration
    {
        set_login_visiable( false );

        login_label = new QLabel( "login:" );
        login_edit = new QLineEdit();
        login_edit->setPlaceholderText( "admin" );

        mail_label = new QLabel( "e-mail:" );
        mail_edit = new QLineEdit();
        mail_edit->setPlaceholderText( "admin@adminpost.com" );

        pass_label = new QLabel( "Password:" );
        pass_edit = new QLineEdit();
        pass_edit->setEchoMode( QLineEdit::Password );
        pass_edit->setPlaceholderText( "qwerty12345" );

        pass_ack_label = new QLabel( "Repeat pass" );
        pass_ack_edit = new QLineEdit();
        pass_ack_edit->setEchoMode( QLineEdit::Password );
        pass_ack_edit->setPlaceholderText( "Use password longer than 6 characters" );

        sign_up_btn = new QPushButton( "Sign Up" );

        reg_layout = new QFormLayout();
        reg_layout->setVerticalSpacing( 0 );
        reg_layout->setContentsMargins( 10, 10, 5, 0 );

        reg_layout->addRow( login_label, login_edit );
        reg_layout->addRow( mail_label, mail_edit );
        reg_layout->addRow( pass_label, pass_edit );
        reg_layout->addRow( pass_ack_label, pass_ack_edit );
        reg_layout->addRow( sign_up_btn );

        ui->layouts->addLayout( reg_layout );

        set_registr_visiable( false );
        show_login();
    }
    connect( ui->login_button,    SIGNAL( clicked(bool) ),  this, SLOT( show_login()           ) );
    connect( ui->registr_button,  SIGNAL( clicked(bool) ),  this, SLOT( show_registr()         ) );
    connect( ui->sign_in,         SIGNAL( clicked(bool) ),  this, SLOT( process_login_form()   ) );
    connect( sign_up_btn,         SIGNAL( clicked(bool) ),  this, SLOT( process_registr_form() ) );
    this->resize( 400, 300 );
    show();
}

Authentification::~Authentification() {
    delete ui;
}

void Authentification::show_login_error() {    
    QMessageBox::warning(this, "Упс", "Невозможно войти с предоставленными данными");
    set_buttons_block( false );
}

void Authentification::show_registr_error(int codes) {
    QMessageBox::warning(this, "Упc^3", "Вот какие-то ошибки(1 в 2-ичной записи числа), разбирайся сам: " + QString(codes));
    set_buttons_block( false );
}


void Authentification::show_login() {
    qDebug() << "Show login";
    reg_layout->setVerticalSpacing( 0 );
    ui->login_layout->setVerticalSpacing( 10 );
    set_login_visiable( true );
}

void Authentification::show_registr() {
    qDebug() << "Show registr";
    reg_layout->setVerticalSpacing( 10 );
    ui->login_layout->setVerticalSpacing( 0 );
    set_registr_visiable( true );
}

void Authentification::process_login_form() {
    set_buttons_block( true );
    qDebug() << "Process login form";
    QString login = ui->nick_edit->text(),
            pass  = ui->pass_edit->text();

    emit sign_in( login, pass );

    ui->pass_edit->clear();
}

void Authentification::process_registr_form() {
    set_buttons_block( true );
    qDebug() << "Process registr form";

    if ( fields_check() ) {
        QString login = login_edit->text(),
                pass  = pass_edit->text(),
                mail  = mail_edit->text();

        emit sign_up(login, pass, mail);

        pass_ack_edit->clear();
    }
}

void Authentification::set_buttons_block(bool block) {
    ui->sign_in->setEnabled( !block );
    this->sign_up_btn->setEnabled( !block );
}


void Authentification::set_login_visiable(bool vis) {
    if ( vis ) { //
        if ( state == "login" ) {
            qDebug() << "Login visiable not changed";
            return;
        }
        if ( state == "registr" )
            set_registr_visiable( false );
    }
    //qDebug() << "Set login visiable - " << vis;
    state = vis ? "login" : "clean";
    ui->remember->setVisible( vis );
    ui->sign_in->setVisible( vis );
    ui->pass_edit->setVisible( vis );
    ui->pass_label->setVisible( vis );
    ui->nick_edit->setVisible( vis );
    ui->nick_label->setVisible( vis );
    /* template:
      ->setVisible( vis );
    ui->
    */
}

void Authentification::set_registr_visiable(bool vis) {
    if ( vis ) {
        if ( state == "registr" ) {
            qDebug() << "Registr visiable not changed";
            return;
        }
        if ( state == "login" )
            set_login_visiable( false );
    }
    //qDebug() << "Set registr visiable - " << vis;
    state = vis ? "registr" : "clean";
    this->sign_up_btn->setVisible( vis );
    this->pass_ack_edit->setVisible( vis );
    this->pass_ack_label->setVisible( vis );
    this->pass_edit->setVisible( vis );
    this->pass_label->setVisible( vis );
    this->mail_edit->setVisible( vis );
    this->mail_label->setVisible( vis );
    this->login_edit->setVisible( vis );
    this->login_label->setVisible( vis );
}

void Authentification::show_error(int code) {
    QString title, text;
    switch ( code ) {
        case 0:
            title = "";
            text = "";
            break;
        case 1:
            title = "";
            text = "";
            break;
        case 2:
            title = "";
            text = "";
            break;
        case 3:
            title = "";
            text = "";
            break;
    }
    QMessageBox::warning( this, title, text );
}

bool Authentification::fields_check() {
    return true;
}
