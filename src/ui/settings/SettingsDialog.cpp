#include "SettingsDialog.hpp"

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

#include "SettingsData.hpp"

SettingsDialog::SettingsDialog(std::shared_ptr<SettingsData>	settingsData,
			       bool				isTableActive,
			       QWidget *			parent) :
	m_isTableActive(isTableActive),
	m_settingsData(settingsData)
{
	setWindowTitle(tr("Settings"));

	m_rulesetBox = new QComboBox;
	m_rulesetBox->addItem("Pathfinder 1E", SettingsData::Ruleset::PATHFINDER_1E);
	m_rulesetBox->addItem("Pathfinder 2E", SettingsData::Ruleset::PATHFINDER_2E);
	m_rulesetBox->addItem("DnD 3.5E", SettingsData::Ruleset::DND_3_5E);
	m_rulesetBox->addItem("DnD 5E", SettingsData::Ruleset::DND_5E);
	m_rulesetBox->setCurrentIndex(m_settingsData->m_ruleset);

	auto *const rulesetLayout = new QHBoxLayout;
	rulesetLayout->setAlignment(Qt::AlignLeft);
	rulesetLayout->addWidget(new QLabel(tr("Ruleset:")));
	rulesetLayout->addWidget(m_rulesetBox);

	m_rollTieBox = new QCheckBox;
	m_rollTieBox->setChecked(m_settingsData->m_rollAutomatically);
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

	connect(okButton, &QPushButton::clicked, this, &SettingsDialog::okClicked);
	connect(applyButton, &QPushButton::clicked, this, &SettingsDialog::applyClicked);
	connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}


bool
SettingsDialog::applyClicked()
{
	if (m_rulesetBox->currentIndex() != m_settingsData->m_ruleset || m_rollTieBox->isChecked() != m_settingsData->m_rollAutomatically) {
		if (m_isTableActive) {
			auto const reply = QMessageBox::critical(
				this,
				tr("Combat active!"),
				tr("You changed the ruleset while a Combat is active. Please save and exit "
				   "the current Combat before changing the ruleset."));
			return false;
		}
		m_settingsData->writeSettings(m_rulesetBox->currentIndex(), m_rollTieBox->isChecked());
		return true;
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
