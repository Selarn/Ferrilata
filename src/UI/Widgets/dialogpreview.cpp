#include "dialogpreview.h"
#include "ui_dialogpreview.h"

DialogPreview::DialogPreview(QWidget *parent): QWidget(parent), ui(new Ui::DialogPreview) {
    ui->setupUi(this);
    setAutoFillBackground( true );
}

DialogPreview::~DialogPreview() {
    delete ui;
}

DialogPreviewCrutch::DialogPreviewCrutch(qint32 user_id, const QString& username, QWidget *parent): QPushButton(username, parent) {
    setMaximumSize(210, 50);
    setAutoFillBackground( true );
    reciver_id = user_id;
    connect( this, &DialogPreviewCrutch::clicked, this, &DialogPreviewCrutch::this_button_was_clicked );
}

DialogPreviewCrutch::~DialogPreviewCrutch() {

}

void DialogPreviewCrutch::this_button_was_clicked(bool trash) {
    trash = !trash;
    emit dialog_selected( reciver_id );
}
