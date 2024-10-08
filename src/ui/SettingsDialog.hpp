#pragma once

#include <QDialog>
#include <QPointer>

class AdditionalSettings;
class RuleSettings;

class QCheckBox;
class QComboBox;

// Dialog for the main application settings
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit
    SettingsDialog(AdditionalSettings& AdditionalSettings,
                   RuleSettings&       RuleSettings,
                   bool                isTableActive,
                   QWidget*            parent = 0);

private slots:
    void
    resetToDefaultClicked();

    void
    applyClicked();

    void
    okClicked();

private:
    QPointer<QComboBox> m_rulesetBox;
    QPointer<QCheckBox> m_rollTieBox;
    QPointer<QCheckBox> m_indicatorMultipleCharsBox;
    QPointer<QCheckBox> m_rollIniMultipleCharsBox;
    QPointer<QCheckBox> m_modToIniCharsBox;

    RuleSettings& m_ruleSettings;
    AdditionalSettings& m_additionalSettings;
};
