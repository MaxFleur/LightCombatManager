#include "MainSettingsDialog.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "MainSettings.hpp"

MainSettingsDialog::MainSettingsDialog(std::shared_ptr<MainSettings>	MainSettings,
				       bool				isTableActive,
				       QWidget *			parent) :
	m_isTableActive(isTableActive),
	m_mainSettings(MainSettings)
{
	setWindowTitle(tr("Settings"));

	m_rulesetBox = new QComboBox;
	m_rulesetBox->addItem("Pathfinder 1E/D&D 3.5E", MainSettings::Ruleset::PATHFINDER_1E_DND_3_5E);
	m_rulesetBox->addItem("Pathfinder 2E", MainSettings::Ruleset::PATHFINDER_2E);
	m_rulesetBox->addItem("D&D 5E", MainSettings::Ruleset::DND_5E);
	m_rulesetBox->addItem("D&D 3.0E", MainSettings::Ruleset::DND_3_0E);
	m_rulesetBox->addItem("Starfinder", MainSettings::Ruleset::STARFINDER);
	m_rulesetBox->setToolTip(tr("The ruleset determines the Table sorting rules and the available status effects."));
	m_rulesetBox->setCurrentIndex(m_mainSettings->ruleset);

	auto *const rulesetLayout = new QHBoxLayout;
	rulesetLayout->setAlignment(Qt::AlignLeft);
	rulesetLayout->addWidget(new QLabel(tr("Ruleset:")));
	rulesetLayout->addWidget(m_rulesetBox);

	m_rollTieBox = new QCheckBox;
	m_rollTieBox->setChecked(m_mainSettings->rollAutomatical);
	auto *const rollTieLabel = new QLabel(tr("Roll automatically for tie"));
	rollTieLabel->setToolTip(tr("If a tie occurs while Characters are generated for a Combat, the app will "
				    "automatically decide the turn order."));

	auto *const rollTieLayout = new QHBoxLayout;
	rollTieLayout->setAlignment(Qt::AlignLeft);
	rollTieLayout->addWidget(m_rollTieBox);
	rollTieLayout->addWidget(rollTieLabel);

	auto *const rulesLayout = new QVBoxLayout;
	rulesLayout->addLayout(rulesetLayout);
	rulesLayout->addLayout(rollTieLayout);

	auto *const rulesetGroupBox = new QGroupBox(tr("Rules"));
	rulesetGroupBox->setLayout(rulesLayout);

	auto *const buttonBox = new QDialogButtonBox;
	auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
	auto *const applyButton = buttonBox->addButton(QDialogButtonBox::Apply);
	auto *const cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

	auto *const mainLayout = new QVBoxLayout;
	mainLayout->addWidget(rulesetGroupBox);
	mainLayout->addWidget(buttonBox);
	setLayout(mainLayout);

	connect(okButton, &QPushButton::clicked, this, &MainSettingsDialog::okClicked);
	connect(applyButton, &QPushButton::clicked, this, &MainSettingsDialog::applyClicked);
	connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}


bool
MainSettingsDialog::applyClicked()
{
	if (m_rulesetBox->currentIndex() != m_mainSettings->ruleset || m_rollTieBox->isChecked() != m_mainSettings->rollAutomatical) {
		// It could be dangerous to change the combat rules while a combat is active, so abort instead
		if (m_isTableActive) {
			auto const reply = QMessageBox::critical(
				this,
				tr("Combat active!"),
				tr("You changed the ruleset while a Combat is active. Please save and exit "
				   "the current Combat before changing the ruleset."));
			return false;
		}
		m_mainSettings->write(m_rulesetBox->currentIndex(), m_rollTieBox->isChecked());
		return true;
	}
	return true;
}


void
MainSettingsDialog::okClicked()
{
	if (applyClicked()) {
		QDialog::accept();
	}
}
