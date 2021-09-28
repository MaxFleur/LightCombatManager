#include "../../include/GUI/CharacterCreationWidget.hpp"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

CharacterCreationWidget::CharacterCreationWidget(CharacterHandlerRef charSort, QWidget *parent)
	: m_char(charSort)
{
	auto *bottomWidget = new QWidget();
	bottomWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Label for the headline
	auto *headLabel = new QLabel("Please enter the character stats.");
	headLabel->setStyleSheet("font: 12pt; font-weight: bold;");
	headLabel->setFrameStyle(QFrame::NoFrame);
	headLabel->setAlignment(Qt::AlignLeft);
	headLabel->setAlignment(Qt::AlignTop);

	// Labels for character stats
	auto *nameLabel = new QLabel("Name:");
	nameLabel->setFrameStyle(QFrame::NoFrame);
	nameLabel->setAlignment(Qt::AlignLeft);
	nameLabel->setToolTip("Set the name of the character.");

	auto *initLabel = new QLabel("Initiative:");
	initLabel->setFrameStyle(QFrame::NoFrame);
	initLabel->setAlignment(Qt::AlignLeft);
	initLabel->setToolTip("Set the initiative. This includes ALL modifiers.");

	auto *initModifierLabel = new QLabel("Init modifier:");
	initModifierLabel->setFrameStyle(QFrame::NoFrame);
	initModifierLabel->setAlignment(Qt::AlignLeft);
	initModifierLabel->setToolTip("Set the modificator of the initiative.");

	auto *hpLabel = new QLabel("HP:");
	hpLabel->setFrameStyle(QFrame::NoFrame);
	hpLabel->setAlignment(Qt::AlignLeft);
	hpLabel->setToolTip("Set the HP of this character. This value is optional.");

	auto *isNPCLabel = new QLabel("Is NPC:");
	isNPCLabel->setFrameStyle(QFrame::NoFrame);
	isNPCLabel->setAlignment(Qt::AlignLeft);
	isNPCLabel->setToolTip("Set if this character is an NPC or not.");

	auto *addInfoLabel = new QLabel("Additional information:");
	addInfoLabel->setFrameStyle(QFrame::NoFrame);
	addInfoLabel->setAlignment(Qt::AlignLeft);
	addInfoLabel->setToolTip("Set some additional information. This is optional.");

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
	m_saveButton = new QPushButton("Save and create new character");
	m_saveButton->setFixedWidth(200);
	m_saveButton->setToolTip("Save this character and create another one.");

	m_finishButton = new QPushButton("Save and finish");
	m_finishButton->setFixedWidth(110);
	m_finishButton->setToolTip("Finish the character creation. Once you're finished, you can't go back.");

	m_resetButton = new QPushButton("Reset");
	m_resetButton->setFixedWidth(60);
	m_resetButton->setToolTip("Reset the current character.");

	m_cancelButton = new QPushButton("Cancel");
	m_cancelButton->setFixedWidth(60);
	m_cancelButton->setToolTip("Cancel the entire character creation. All changes will be lost.");

	// Layouts
	auto *charInfoLayout = new QHBoxLayout;
	charInfoLayout->addWidget(nameLabel);
	charInfoLayout->addWidget(m_nameEdit);
	charInfoLayout->addWidget(initLabel);
	charInfoLayout->addWidget(m_initBox);
	charInfoLayout->addWidget(initModifierLabel);
	charInfoLayout->addWidget(m_initModifierBox);
	charInfoLayout->addWidget(hpLabel);
	charInfoLayout->addWidget(m_hpBox);
	charInfoLayout->addWidget(isNPCLabel);
	charInfoLayout->addWidget(m_isNPCBox);
	charInfoLayout->setAlignment(Qt::AlignTop);

	auto *addInfoLayout = new QHBoxLayout;
	addInfoLayout->addWidget(addInfoLabel);
	addInfoLayout->addWidget(m_addInfoEdit);
	addInfoLayout->setAlignment(Qt::AlignTop);

	// Align the buttons to the right side using a spacer
	auto *spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(spacer);
	buttonLayout->addWidget(m_saveButton);
	buttonLayout->addWidget(m_finishButton);
	buttonLayout->addWidget(m_resetButton);
	buttonLayout->addWidget(m_cancelButton);

	auto *mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(5, 5, 5, 5);
	mainLayout->addSpacing(10);
	mainLayout->addWidget(headLabel);
	mainLayout->addSpacing(30);
	mainLayout->addLayout(charInfoLayout);
	mainLayout->addSpacing(10);
	mainLayout->addLayout(addInfoLayout);
	mainLayout->addSpacing(10);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addWidget(bottomWidget);

	connect(m_saveButton, &QPushButton::clicked, this, &CharacterCreationWidget::saveAndCreateNewCharacter);
	connect(m_resetButton, &QPushButton::clicked, this, &CharacterCreationWidget::resetCharacter);
	connect(
		m_cancelButton,
		&QPushButton::clicked,
		this,
		[this] () {
			emit cancel();
		});
	connect(
		m_finishButton,
		&QPushButton::clicked,
		this,
		[this] () {
			emit finish();
		});

	// Default character values
	resetCharacter();
}


// Store the last created character. After this, the table widget will be created.
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
	// Check if character has a name
	if (m_nameEdit->text().isEmpty()) {
		QMessageBox::StandardButton reply = QMessageBox::warning(
			this,
			"Creation not possible!",
			"No name has been set. Please set at least a name before storing the character!");
		return;
	}
	storeCharacter();
	resetCharacter();
}


// Reset the current character
void
CharacterCreationWidget::resetCharacter()
{
	// Reset all displayed values
	m_nameEdit->clear();
	m_initBox->setValue(0);
	m_initModifierBox->setValue(0);
	m_isNPCBox->setChecked(false);
	m_hpBox->setValue(0);
	m_addInfoEdit->clear();
}


void
CharacterCreationWidget::storeCharacter()
{
	m_char->storeCharacter(
		m_nameEdit->text(),
		m_initBox->value(),
		m_initModifierBox->value(),
		m_isNPCBox->isChecked(),
		m_hpBox->value(),
		m_addInfoEdit->text());
}
