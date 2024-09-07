#include "SettingsDialog.hpp"

#include "AdditionalSettings.hpp"
#include "RuleSettings.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(AdditionalSettings& AdditionalSettings,
                               RuleSettings&       RuleSettings,
                               bool                isTableActive,
                               QWidget*            parent) :
    QDialog(parent),
    m_ruleSettings(RuleSettings),
    m_additionalSettings(AdditionalSettings)
{
    setWindowTitle(tr("Settings"));

    auto* const rulesLabel = new QLabel(tr("<b>Rules</b>"));
    rulesLabel->setEnabled(!isTableActive);
    auto* const rulesetLabel = new QLabel(tr("Ruleset:"));

    m_rulesetBox = new QComboBox;
    m_rulesetBox->addItem("Pathfinder 1E/D&D 3.5E", RuleSettings::Ruleset::PATHFINDER_1E_DND_35E);
    m_rulesetBox->addItem("Pathfinder 2E", RuleSettings::Ruleset::PATHFINDER_2E);
    m_rulesetBox->addItem("D&D 5E", RuleSettings::Ruleset::DND_5E);
    m_rulesetBox->addItem("D&D 3.0E", RuleSettings::Ruleset::DND_30E);
    m_rulesetBox->addItem("Starfinder", RuleSettings::Ruleset::STARFINDER);
    m_rulesetBox->setToolTip(tr("The ruleset determines the Table sorting rules and the available status effects."));
    m_rulesetBox->setCurrentIndex(m_ruleSettings.ruleset);

    auto *const rulesetLayout = new QHBoxLayout;
    rulesetLayout->setAlignment(Qt::AlignLeft);
    rulesetLayout->addWidget(rulesetLabel);
    rulesetLayout->addWidget(m_rulesetBox);
    rulesetLabel->setEnabled(!isTableActive);
    m_rulesetBox->setEnabled(!isTableActive);

    m_rollTieBox = new QCheckBox;
    m_rollTieBox->setChecked(m_ruleSettings.rollAutomatical);
    m_rollTieBox->setText(tr("Roll automatically for tie"));
    m_rollTieBox->setToolTip(tr("If a tie occurs while Characters are generated for a Combat,\n"
                                "the app will automatically decide the turn order."));
    m_rollTieBox->setEnabled(!isTableActive);

    auto* const additionalLabel = new QLabel(tr("<b>Additional:</b>"));

    m_indicatorMultipleCharsBox = new QCheckBox(tr("Set Indicator for multiple Characters"));
    m_indicatorMultipleCharsBox->setChecked(m_additionalSettings.indicatorMultipleChars);
    m_indicatorMultipleCharsBox->setToolTip(tr("If a Character is added to the table multiple times,\n"
                                               "an additional indicator is appended to each Character."));

    m_rollIniMultipleCharsBox = new QCheckBox(tr("Roll Initiative for multiple Characters"));
    m_rollIniMultipleCharsBox->setChecked(m_additionalSettings.rollIniMultipleChars);
    m_rollIniMultipleCharsBox->setToolTip(tr("If a Character is added to the table multiple times,\n"
                                             "the Initiative is rerolled for each duplicate."));

    m_modToIniCharsBox = new QCheckBox(tr("Add Mod Value to Initiative Value"));
    m_modToIniCharsBox->setChecked(m_additionalSettings.modAddedToIni);
    m_modToIniCharsBox->setToolTip(tr("If the mod value is adjusted in the Character Dialog,\n"
                                      "this value will be added to the Initiative value."));

    auto* const resetToDefaultButton = new QPushButton(tr("Reset to Defaults"));
    resetToDefaultButton->setEnabled(!isTableActive);

    auto *const buttonBox = new QDialogButtonBox;
    auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    auto *const applyButton = buttonBox->addButton(QDialogButtonBox::Apply);
    auto *const cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

    auto* const resetToDefaultButtonLayout = new QHBoxLayout;
    resetToDefaultButtonLayout->addStretch();
    resetToDefaultButtonLayout->addWidget(resetToDefaultButton);

    auto *const mainLayout = new QVBoxLayout;
    mainLayout->addWidget(rulesLabel);
    mainLayout->addLayout(rulesetLayout);
    mainLayout->addWidget(m_rollTieBox);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(additionalLabel);
    mainLayout->addWidget(m_indicatorMultipleCharsBox);
    mainLayout->addWidget(m_rollIniMultipleCharsBox);
    mainLayout->addWidget(m_modToIniCharsBox);
    mainLayout->addLayout(resetToDefaultButtonLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    connect(resetToDefaultButton, &QPushButton::clicked, this, &SettingsDialog::resetToDefaultClicked);
    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::okClicked);
    connect(applyButton, &QPushButton::clicked, this, &SettingsDialog::applyClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}


void
SettingsDialog::resetToDefaultClicked()
{
    m_rulesetBox->setCurrentIndex(0);
    m_rollTieBox->setChecked(false);
    m_indicatorMultipleCharsBox->setChecked(true);
    m_rollIniMultipleCharsBox->setChecked(false);
    m_modToIniCharsBox->setChecked(true);
}


void
SettingsDialog::applyClicked()
{
    if (m_rulesetBox->currentIndex() != m_ruleSettings.ruleset ||
        m_rollTieBox->isChecked() != m_ruleSettings.rollAutomatical) {
        m_ruleSettings.write(m_rulesetBox->currentIndex(), m_rollTieBox->isChecked());
    }
    if (m_indicatorMultipleCharsBox->isChecked() != m_additionalSettings.indicatorMultipleChars ||
        m_rollIniMultipleCharsBox->isChecked() != m_additionalSettings.rollIniMultipleChars ||
        m_modToIniCharsBox->isChecked() != m_additionalSettings.modAddedToIni) {
        m_additionalSettings.write(m_indicatorMultipleCharsBox->isChecked(), m_rollIniMultipleCharsBox->isChecked(),
                                   m_modToIniCharsBox->isChecked());
    }
}


void
SettingsDialog::okClicked()
{
    applyClicked();
    QDialog::accept();
}
