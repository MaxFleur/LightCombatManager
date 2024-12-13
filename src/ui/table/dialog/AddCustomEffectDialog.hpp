#pragma once

#include <QDialog>

// Dialog used to add effects
class AddCustomEffectDialog : public QDialog {
    Q_OBJECT

public:
    explicit
    AddCustomEffectDialog(const QList<QString>& otherEffects,
                          QWidget*              parent = 0);

    [[nodiscard]] QString&
    getName()
    {
        return m_name;
    }

private:
    QString m_name;
};
