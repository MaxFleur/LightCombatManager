#pragma once

#include <QDialog>
#include <QPointer>

class RuleSettings;

class QCheckBox;
class QComboBox;

// Dialog for the main application settings
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit
    SettingsDialog(std::shared_ptr<RuleSettings> RuleSettings,
                   bool                          isTableActive,
                   QWidget*                      parent = 0);

private slots:
    bool
    applyClicked();

    void
    okClicked();

private:
    QPointer<QComboBox> m_rulesetBox;
    QPointer<QCheckBox> m_rollTieBox;

    std::shared_ptr<RuleSettings> m_ruleSettings;
    const bool m_isTableActive;
};
