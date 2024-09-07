#pragma once

#include <QDialog>

// Dialog used to add or subtract HP from multiple characters at once
class ChangeHPDialog : public QDialog {
    Q_OBJECT

public:
    explicit
    ChangeHPDialog(QWidget* parent = 0);

    [[nodiscard]] int
    getHPValue()
    {
        return m_hpValue;
    }

private:
    int m_hpValue = 0;
};
