#ifndef DIALOGPREVIEW_H
#define DIALOGPREVIEW_H

#include <QPushButton>
#include <QWidget>

namespace Ui {
    class DialogPreview;
}

class DialogPreview : public QWidget {
        Q_OBJECT

    public:
        explicit DialogPreview(QWidget *parent = nullptr);
        ~DialogPreview();

    signals:
        void dialog_selected(qint32 user_id);

    private:
        Ui::DialogPreview *ui;
};

class DialogPreviewCrutch: public QPushButton {
        Q_OBJECT

        qint32 reciver_id;

    signals:
        void dialog_selected( qint32 user_id );

    public:
        explicit DialogPreviewCrutch(qint32 user_id, const QString& username, QWidget *parent = nullptr);
        ~DialogPreviewCrutch();

    protected slots:
        void this_button_was_clicked( bool trash );
};

#endif // DIALOGPREVIEW_H
