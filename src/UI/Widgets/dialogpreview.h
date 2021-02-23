#ifndef DIALOGPREVIEW_H
#define DIALOGPREVIEW_H

#include "src/typedefs.h"

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
        void dialog_selected(UserID user_id);

    private:
        Ui::DialogPreview *ui;
};

class DialogPreviewCrutch: public QPushButton {
        Q_OBJECT

        UserID reciver_id;

    signals:
        void dialog_selected( UserID user_id );

    public:
        explicit DialogPreviewCrutch(UserID user_id, const QString& username, QWidget *parent = nullptr);
        ~DialogPreviewCrutch();

    protected slots:
        void this_button_was_clicked( bool trash );
};

#endif // DIALOGPREVIEW_H
