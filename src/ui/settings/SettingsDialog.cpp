#include "SettingsDialog.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "SettingsData.hpp"

SettingsDialog::SettingsDialog(QWidget *parent)
{
	setWindowTitle(tr("Settings"));

	auto *const rulesetBox = new QComboBox;
	rulesetBox->addItem("Pathfinder 1E", SettingsData::Ruleset::PATHFINDER_1E);
	rulesetBox->addItem("Pathfinder 2E", SettingsData::Ruleset::PATHFINDER_2E);
	rulesetBox->addItem("DnD 3.5E", SettingsData::Ruleset::DND_3_5E);
	rulesetBox->addItem("DnD 5E", SettingsData::Ruleset::DND_5E);

	auto *const rulesetLayout = new QHBoxLayout();
	rulesetLayout->setAlignment(Qt::AlignLeft);
	rulesetLayout->addWidget(new QLabel(tr("Ruleset:")));
	rulesetLayout->addWidget(rulesetBox);

	auto *const rollTieBox = new QCheckBox;

	auto *const rollTieLayout = new QHBoxLayout();
	rollTieLayout->setAlignment(Qt::AlignLeft);
	rollTieLayout->addWidget(rollTieBox);
	rollTieLayout->addWidget(new QLabel(tr("Roll automatically for tie")));

	auto *const rulesLayout = new QVBoxLayout();
	rulesLayout->addLayout(rulesetLayout);
	rulesLayout->addLayout(rollTieLayout);

	auto * const rulesetGroupBox = new QGroupBox(tr("Rules"), this);
	rulesetGroupBox->setLayout(rulesLayout);

	auto * const buttonBox = new QDialogButtonBox(this);
	auto * const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
	auto * const applyButton = buttonBox->addButton(QDialogButtonBox::Apply);
	auto * const cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);

	auto *const mainLayout = new QVBoxLayout();
	mainLayout->addWidget(rulesetGroupBox);
	mainLayout->addWidget(buttonBox);
	setLayout(mainLayout);

	connect(okButton, &QPushButton::clicked, this, &SettingsDialog::okClicked);
	connect(applyButton, &QPushButton::clicked, this, &SettingsDialog::applyClicked);
	connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}


void
SettingsDialog::applyClicked()
{
}


void
SettingsDialog::okClicked()
{
}
