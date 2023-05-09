#include "SettingsDialog.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "AdditionalSettings.hpp"
#include "RuleSettings.hpp"

SettingsDialog::SettingsDialog(std::shared_ptr<AdditionalSettings> AdditionalSettings,
                               std::shared_ptr<RuleSettings>       RuleSettings,
                               bool                                isTableActive,
                               QWidget*                            parent) :
    m_isTableActive(isTableActive),
    m_ruleSettings(RuleSettings),
    m_additionalSettings(AdditionalSettings)
{
    setWindowTitle(tr("Settings"));

    auto* const rulesLabel = new QLabel(tr("<b>Rules</b>"));

    m_rulesetBox = new QComboBox;
    m_rulesetBox->addItem("Pathfinder 1E/D&D 3.5E", RuleSettings::Ruleset::PATHFINDER_1E_DND_35E);
    m_rulesetBox->addItem("Pathfinder 2E", RuleSettings::Ruleset::PATHFINDER_2E);
    m_rulesetBox->addItem("D&D 5E", RuleSettings::Ruleset::DND_5E);
    m_rulesetBox->addItem("D&D 3.0E", RuleSettings::Ruleset::DND_30E);
    m_rulesetBox->addItem("Starfinder", RuleSettings::Ruleset::STARFINDER);
    m_rulesetBox->setToolTip(tr("The ruleset determines the Table sorting rules and the available status effects."));
    m_rulesetBox->setCurrentIndex(m_ruleSettings->ruleset);

    auto *const rulesetLayout = new QHBoxLayout;
    rulesetLayout->setAlignment(Qt::AlignLeft);
    rulesetLayout->addWidget(new QLabel(tr("Ruleset:")));
    rulesetLayout->addWidget(m_rulesetBox);

    m_rollTieBox = new QCheckBox;
    m_rollTieBox->setChecked(m_ruleSettings->rollAutomatical);
    m_rollTieBox->setText(tr("Roll automatically for tie"));
    m_rollTieBox->setToolTip(tr("If a tie occurs while Characters are generated for a Combat,\n"
                                "the app will automatically decide the turn order."));

    auto* const additionalLabel = new QLabel(tr("<b>Additional:</b>"));

    m_indicatorMultipleCharsBox = new QCheckBox(tr("Set Indicator for multiple Characters"));
    m_indicatorMultipleCharsBox->setChecked(m_additionalSettings->indicatorMultipleChars);
    m_indicatorMultipleCharsBox->setToolTip(tr("If a Character is added to the table multiple times,\n"
                                               "an additional indicator is appended to each Character."));

    m_rollIniMultipleCharsBox = new QCheckBox(tr("Roll Initiative for multiple Characters"));
    m_rollIniMultipleCharsBox->setChecked(m_additionalSettings->rollIniMultipleChars);
    m_rollIniMultipleCharsBox->setToolTip(tr("If a Character is added to the table multiple times,\n"
                                             "the Initiative is rerolled for each duplicate."));

    auto *const buttonBox = new QDialogButtonBox;
    auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    auto *const applyButton = buttonBox->addButton(QDialogButtonBox::Apply);
    auto *const cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    auto *const mainLayout = new QVBoxLayout;
    mainLayout->addWidget(rulesLabel);
    mainLayout->addLayout(rulesetLayout);
    mainLayout->addWidget(m_rollTieBox);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(additionalLabel);
    mainLayout->addWidget(m_indicatorMultipleCharsBox);
    mainLayout->addWidget(m_rollIniMultipleCharsBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::okClicked);
    connect(applyButton, &QPushButton::clicked, this, &SettingsDialog::applyClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}


bool
SettingsDialog::applyClicked()
{
    if (m_rulesetBox->currentIndex() != m_ruleSettings->ruleset ||
        m_rollTieBox->isChecked() != m_ruleSettings->rollAutomatical) {
        // It could be dangerous to change the combat rules while a combat is active, so abort
        if (m_isTableActive) {
            auto const reply = QMessageBox::critical(this, tr("Combat active!"),
                                                     tr("You changed the ruleset while a Combat is active. Please save and exit "
                                                        "the current Combat before changing the ruleset."));
            return false;
        }
        m_ruleSettings->write(m_rulesetBox->currentIndex(), m_rollTieBox->isChecked());
        return true;
    }
    if (m_indicatorMultipleCharsBox->isChecked() != m_additionalSettings->indicatorMultipleChars ||
        m_rollIniMultipleCharsBox->isChecked() != m_additionalSettings->rollIniMultipleChars) {
        m_additionalSettings->write(m_indicatorMultipleCharsBox->isChecked(), m_rollIniMultipleCharsBox->isChecked());
    }
    return true;
}


void
SettingsDialog::okClicked()
{
    if (applyClicked()) {
        QDialog::accept();
    }
}
