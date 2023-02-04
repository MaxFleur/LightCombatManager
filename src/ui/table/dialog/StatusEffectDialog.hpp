#pragma once

#include <QDialog>
#include <QPointer>

class QLineEdit;
class QListWidget;

class RuleSettings;

// Dialog used to add certain status effects to characters
class StatusEffectDialog : public QDialog {
    Q_OBJECT

public:
    explicit
    StatusEffectDialog(std::shared_ptr<RuleSettings> RuleSettings,
                       QWidget *                     parent = 0);

    [[nodiscard]] QString
    getEffect() const
    {
        return m_effect;
    }

private slots:
    void
    okButtonClicked();

private:
    void
    findEffect(const QString& filter);

private:
    QPointer<QListWidget> m_listWidget;
    QPointer<QLineEdit> m_lineEdit;

    QString m_effect;

    std::shared_ptr<RuleSettings> m_ruleSettings;
};
