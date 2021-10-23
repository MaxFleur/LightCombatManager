#include "../../include/GUI/CharacterCreationWidget.hpp"

#include <QApplication>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

#include "../../include/GUI/StatusEffectDialog.hpp"

CharacterCreationWidget::CharacterCreationWidget(CharacterHandlerRef charSort, QWidget *parent)
	: m_char(charSort)
{
	// Label for the headline
	auto *const headLabel = new QLabel(tr("Please enter the Character Stats."));
	headLabel->setStyleSheet("font: 12pt; font-weight: bold;");

	// Labels for character stats
	auto *const nameLabel = new QLabel(tr("Name:"));
	nameLabel->setToolTip(tr("Set the name of the Character. A Character can't be stored without a name."));

	auto *const iniLabel = new QLabel(tr("Initiative:"));
	iniLabel->setToolTip(tr("Set the initiative, including all modifiers. Optional."));

	auto *const iniModifierLabel = new QLabel(tr("Init Modifier:"));
	iniModifierLabel->setToolTip(tr("Set the modificator of the initiative. Optional."));

	auto *const hpLabel = new QLabel(tr("HP:"));
	hpLabel->setToolTip(tr("Set the HP of this Character. Optional."));

	auto *const npcLabel = new QLabel(tr("Is Character NPC:"));
	npcLabel->setToolTip(tr("Set if the Character is an NPC. Optional."));

	auto *const addInfoLabel = new QLabel(tr("Additional Information:"));
	addInfoLabel->setToolTip(tr("Set some additional information. Optional."));

	// Main widgets for character creation
	m_nameEdit = new QLineEdit();
	m_iniBox = new QSpinBox();
	m_iniBox->setMinimum(-20);
	m_iniModifierBox = new QSpinBox();
	m_iniModifierBox->setMinimum(-10);
	m_hpBox = new QSpinBox();
	m_hpBox->setRange(-100, 10000);
	m_npcBox = new QCheckBox();
	m_addInfoEdit = new QLineEdit();

	// Buttons
	m_saveButton = new QPushButton(tr("Save Character"));
	m_saveButton->setToolTip(tr("Save this Character and reset Layout to add another one."));

	m_finishButton = new QPushButton(tr("Finish"));
	m_finishButton->setToolTip(tr("Finish the Character Creation. You can reedit the Combat later."));

	m_resetButton = new QPushButton(tr("Reset Layout"));
	m_resetButton->setToolTip(tr("Reset the Layout values."));

	auto *const addStatusEffectButton = new QPushButton(tr("Add Status Effect"));
	addStatusEffectButton->setToolTip(tr("Add a Status Effect to this Character."));

	// Layouts
	auto *const nameInitLayout = new QHBoxLayout;
	nameInitLayout->addWidget(nameLabel);
	nameInitLayout->addWidget(m_nameEdit);
	nameInitLayout->addWidget(iniLabel);
	nameInitLayout->addWidget(m_iniBox);
	nameInitLayout->setAlignment(Qt::AlignTop);

	auto *const modHPisNPCLayout = new QHBoxLayout;
	modHPisNPCLayout->addWidget(iniModifierLabel);
	modHPisNPCLayout->addWidget(m_iniModifierBox);
	modHPisNPCLayout->addWidget(hpLabel);
	modHPisNPCLayout->addWidget(m_hpBox);
	modHPisNPCLayout->addWidget(npcLabel);
	modHPisNPCLayout->addWidget(m_npcBox);

	auto *const spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	modHPisNPCLayout->addWidget(spacer);
	modHPisNPCLayout->setAlignment(Qt::AlignTop);

	auto *const addInfoLayout = new QHBoxLayout;
	addInfoLayout->addWidget(addInfoLabel);
	addInfoLayout->addWidget(m_addInfoEdit);
	addInfoLayout->addWidget(addStatusEffectButton);
	addInfoLayout->setAlignment(Qt::AlignTop);

	auto *const buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(spacer);
	buttonLayout->addWidget(m_saveButton);
	buttonLayout->addWidget(m_finishButton);
	buttonLayout->addWidget(m_resetButton);

	m_cancelButton = new QPushButton(tr("Cancel"));
	m_cancelButton->setToolTip(tr("Cancel the entire Character Creation. All changes will be lost."));
	buttonLayout->addWidget(m_cancelButton);

	auto *const mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(10, 5, 10, 5);
	mainLayout->addSpacing(10);
	mainLayout->addWidget(headLabel);
	mainLayout->addSpacing(10);
	mainLayout->addLayout(nameInitLayout);
	mainLayout->addLayout(modHPisNPCLayout);
	mainLayout->addLayout(addInfoLayout);
	mainLayout->addSpacing(15);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addSpacing(10);

	// Default character values
	resetLayout();

	connect(addStatusEffectButton, &QPushButton::clicked, this, &CharacterCreationWidget::addStatusEffect);
	connect(m_saveButton, &QPushButton::clicked, this, &CharacterCreationWidget::saveAndCreateNewCharacter);
	connect(m_resetButton, &QPushButton::clicked, this, &CharacterCreationWidget::resetLayout);
	connect(
		m_finishButton,
		&QPushButton::clicked,
		this,
		[this] () {
			emit finish();
		});
	connect(
		m_cancelButton,
		&QPushButton::clicked,
		this,
		[this] () {
			emit cancel();
		});
}


// Save the current character and reset to create another one
void
CharacterCreationWidget::saveAndCreateNewCharacter()
{
	if (m_nameEdit->text().isEmpty()) {
		auto const reply = QMessageBox::warning(
			this,
			tr("Creation not possible!"),
			tr("No name has been set. Please set a name before storing the Character!"));
		return;
	}
	storeCharacter();
	resetLayout();
	// Focus back to name label
	setFocus();
}


// Reset all displayed values
void
CharacterCreationWidget::resetLayout()
{
	m_nameEdit->clear();
	m_iniBox->setValue(0);
	m_iniModifierBox->setValue(0);
	m_hpBox->setValue(0);
	m_npcBox->setChecked(false);
	m_addInfoEdit->clear();
}


void
CharacterCreationWidget::storeCharacter()
{
	m_char->storeCharacter(
		m_nameEdit->text(),
		m_iniBox->value(),
		m_iniModifierBox->value(),
		m_hpBox->value(),
		m_npcBox->isChecked(),
		m_addInfoEdit->text());
}


// Store the last created character. After this, the table widget will be initialized.
void
CharacterCreationWidget::storeLastCharacter()
{
	if (!m_nameEdit->text().isEmpty()) {
		storeCharacter();
	}
}


void
CharacterCreationWidget::addStatusEffect()
{
	// Open dialog
	auto *const dialog = new StatusEffectDialog(this);
	// Lock until dialog is closed
	if (dialog->exec() == QDialog::Accepted) {
		// If accepted, add status effect
		auto itemText = m_addInfoEdit->text();
		if (!itemText.isEmpty() && itemText.back() != " ") {
			itemText += ", ";
		}
		m_addInfoEdit->setText(itemText + dialog->getEffect());
	}
}


void
CharacterCreationWidget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return) {
		event->modifiers() == Qt::ControlModifier ? emit finish() : saveAndCreateNewCharacter();
	}
	if (event->key() == Qt::Key_E) {
		if (event->modifiers() == Qt::ControlModifier) {
			addStatusEffect();
		}
	}
	QWidget::keyPressEvent(event);
}
