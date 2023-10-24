#pragma once

#include <QLineEdit>

class FocusOutLineEdit : public QLineEdit {
    Q_OBJECT

signals:
    void
    focusOut();

protected:
    virtual void
    focusOutEvent(QFocusEvent *event)
    {
        QLineEdit::focusOutEvent(event);
        emit focusOut();
    }
};
