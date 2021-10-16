#include "../../include/GUI/CharacterCreationWidget.hpp"

#include <QApplication>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPointer>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "../../include/GUI/StatusEffectDialog.hpp"

CharacterCreationWidget::CharacterCreationWidget(CharacterHandlerRef charSort, QWidget *parent)
	: m_char(charSort)
{
	auto *const bottomWidget = new QWidget();
	bottomWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Label for the headline
	auto *const headLabel = new QLabel(tr("Please enter the Character Stats."));
	headLabel->setStyleSheet("font: 12pt; font-weight: bold;");
	headLabel->setFrameStyle(QFrame::NoFrame);
	headLabel->setAlignment(Qt::AlignLeft);
	headLabel->setAlignment(Qt::AlignTop);

	// Labels for character stats
	auto *const nameLabel = new QLabel(tr("Name:"));
	nameLabel->setFrameStyle(QFrame::NoFrame);
	nameLabel->setAlignment(Qt::AlignLeft);
	nameLabel->setToolTip(tr("Set the name of the Character. A Character can't be stored without a name."));

	auto *const initLabel = new QLabel(tr("Initiative:"));
	initLabel->setFrameStyle(QFrame::NoFrame);
	initLabel->setAlignment(Qt::AlignLeft);
	initLabel->setToolTip(tr("Set the initiative, including all modifiers. Optional."));

	auto *const initModifierLabel = new QLabel(tr("Init Modifier:"));
	initModifierLabel->setFrameStyle(QFrame::NoFrame);
	initModifierLabel->setAlignment(Qt::AlignLeft);
	initModifierLabel->setToolTip(tr("Set the modificator of the initiative. Optional."));

	auto *const hpLabel = new QLabel(tr("HP:"));
	hpLabel->setFrameStyle(QFrame::NoFrame);
	hpLabel->setAlignment(Qt::AlignLeft);
	hpLabel->setToolTip(tr("Set the HP of this Character. Optional."));

	auto *const isNPCLabel = new QLabel(tr("Is Character NPC:"));
	isNPCLabel->setFrameStyle(QFrame::NoFrame);
	isNPCLabel->setAlignment(Qt::AlignLeft);
	isNPCLabel->setToolTip(tr("Set if the Character is an NPC. Optional."));

	auto *const addInfoLabel = new QLabel(tr("Additional Information:"));
	addInfoLabel->setFrameStyle(QFrame::NoFrame);
	addInfoLabel->setAlignment(Qt::AlignLeft);
	addInfoLabel->setToolTip(tr("Set some additional information. Optional."));

	// Main widgets for character creation
	m_nameEdit = new QLineEdit();
	m_initBox = new QSpinBox();
	m_initBox->setMinimum(-20);
	m_initModifierBox = new QSpinBox();
	m_initModifierBox->setMinimum(-10);
	m_hpBox = new QSpinBox();
	m_hpBox->setRange(-100, 10000);
	m_isNPCBox = new QCheckBox();
	m_addInfoEdit = new QLineEdit();

	// Buttons
	m_saveButton = new QPushButton(tr("Save Character"));
	m_saveButton->setToolTip(tr("Save this Character. After this you can create another Character."));

	m_finishButton = new QPushButton(tr("Finish"));
	m_finishButton->setToolTip(tr("Finish the Character Creation. You can reedit the Combat later."));

	m_resetButton = new QPushButton(tr("Reset Character"));
	m_resetButton->setToolTip(tr("Reset the current Character."));

	auto *const addStatusEffectButton = new QPushButton(tr("Add Status Effect"));
	addStatusEffectButton->setToolTip(tr("Add a Status Effect for this Character."));

	// Layouts
	auto *const nameInitLayout = new QHBoxLayout;
	nameInitLayout->addWidget(nameLabel);
	nameInitLayout->addWidget(m_nameEdit);
	nameInitLayout->addWidget(initLabel);
	nameInitLayout->addWidget(m_initBox);
	nameInitLayout->setAlignment(Qt::AlignTop);

	auto *const modHPisNPCLayout = new QHBoxLayout;
	modHPisNPCLayout->addWidget(initModifierLabel);
	modHPisNPCLayout->addWidget(m_initModifierBox);
	modHPisNPCLayout->addWidget(hpLabel);
	modHPisNPCLayout->addWidget(m_hpBox);
	modHPisNPCLayout->addWidget(isNPCLabel);
	modHPisNPCLayout->addWidget(m_isNPCBox);

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
	// If this is the combat edit mode, canceling is permitted
	m_cancelButton = new QPushButton(tr("Cancel"));
	m_cancelButton->setToolTip(tr("Cancel the entire Character Creation. All changes will be lost."));
	buttonLayout->addWidget(m_cancelButton);

	auto *const mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(5, 5, 5, 5);
	mainLayout->addSpacing(10);
	mainLayout->addWidget(headLabel);
	mainLayout->addSpacing(25);
	mainLayout->addLayout(nameInitLayout);
	mainLayout->addLayout(modHPisNPCLayout);
	mainLayout->addLayout(addInfoLayout);
	mainLayout->addSpacing(15);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addWidget(bottomWidget);

	// Default character values
	resetCharacter();

	connect(addStatusEffectButton, &QPushButton::clicked, this, &CharacterCreationWidget::addStatusEffect);
	connect(m_saveButton, &QPushButton::clicked, this, &CharacterCreationWidget::saveAndCreateNewCharacter);
	connect(m_resetButton, &QPushButton::clicked, this, &CharacterCreationWidget::resetCharacter);
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


// Store the last created character. After this, the table widget will be opened.
void
CharacterCreationWidget::storeLastCharacter()
{
	if (!m_nameEdit->text().isEmpty()) {
		storeCharacter();
	}
}


// Save the current character and reset to create another one
void
CharacterCreationWidget::saveAndCreateNewCharacter()
{
	if (m_nameEdit->text().isEmpty()) {
		auto const reply = QMessageBox::warning(
			this,
			tr("Creation not possible!"),
			tr("No name has been set. Please set at least a name before storing the Character!"));
		return;
	}
	storeCharacter();
	resetCharacter();
	setNameFocus();
}


// Reset the current character
void
CharacterCreationWidget::resetCharacter()
{
	// Reset all displayed values
	m_nameEdit->clear();
	m_initBox->setValue(0);
	m_initModifierBox->setValue(0);
	m_hpBox->setValue(0);
	m_isNPCBox->setChecked(false);
	m_addInfoEdit->clear();
}


void
CharacterCreationWidget::storeCharacter()
{
	m_char->storeCharacter(
		m_nameEdit->text(),
		m_initBox->value(),
		m_initModifierBox->value(),
		m_hpBox->value(),
		m_isNPCBox->isChecked(),
		m_addInfoEdit->text());
}


void
CharacterCreationWidget::addStatusEffect()
{
	// Open dialog
	auto *const dialog = new StatusEffectDialog(this);
	// Lock until dialog is closed
	if (dialog->exec() == QDialog::Accepted) {
		// If accepted, set text
		auto const itemText = m_addInfoEdit->text();
		m_addInfoEdit->setText(itemText + dialog->getEffect());
	}
}


void
CharacterCreationWidget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return) {
		saveAndCreateNewCharacter();
	}
	if (event->key() == Qt::Key_E) {
		if (event->modifiers() == Qt::ControlModifier) {
			addStatusEffect();
		}
	}
	QWidget::keyPressEvent(event);
}
