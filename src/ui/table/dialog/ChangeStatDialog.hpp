#pragma once

#include <QDialog>
#include <QVariant>

// Dialog used for HP substraction/addition or text changing
class ChangeStatDialog : public QDialog {
    Q_OBJECT

public:
    explicit
    ChangeStatDialog(bool     changeHP,
                     QWidget* parent = 0);

    [[nodiscard]] QVariant&
    getVariant()
    {
        return m_variant;
    }

private:
    QVariant m_variant;
};
