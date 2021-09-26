#include "../../include/GUI/CharacterCreationWidget.hpp"

// Create the layout at instantiation
CharacterCreationWidget::CharacterCreationWidget(CharacterHandlerRef charSort, QWidget *parent)
	: m_char(charSort)
{
	createWidget();
	connectWidgets();
	// Reset all character values to standard
	resetCharacter();
}


// Set the name of the current character
void CharacterCreationWidget::setName(const QString &name)
{
	m_name = name.toStdString();
}


// Set initiative of the current character
void CharacterCreationWidget::setInitiative(int initiative)
{
	m_initiative = initiative;
}


// Set the initiative modifier of the current character
void CharacterCreationWidget::setModifier(int modifier)
{
	m_modifier = modifier;
}


// Set if the current character is an NPC or not
void CharacterCreationWidget::setIsNPC(bool isNPC)
{
	m_isNPC = isNPC;
}


// Set the current character's hp
void CharacterCreationWidget::setHP(int hp)
{
	m_hp = hp;
}


// Set some additional information for the current character, if needed
void CharacterCreationWidget::setAdditionalInformation(const QString &additionalInf)
{
	m_additionalInf = additionalInf.toStdString();
}


// Save the current character and reset to create another one
void CharacterCreationWidget::saveAndCreateNewCharacter()
{
	// If no name was entered, display a warning message and abort
	if (m_name.empty()) {
		QMessageBox::StandardButton reply = QMessageBox::warning(this,
									 "Creation not possible!",
									 "No name has been set. Please set at least a name before storing the character!");
		return;
	}
	// Otherwise store the character and reset the widgets
	m_char->storeCharacter(m_name, m_initiative, m_modifier, m_isNPC, m_hp, m_additionalInf);
	resetCharacter();
}


// This stores the last created character. After this, the table widget will be created.
void CharacterCreationWidget::storeLastCharacter()
{
	// Don't do anything if no name has been set
	if (!m_name.empty()) {
		m_char->storeCharacter(m_name, m_initiative, m_modifier, m_isNPC, m_hp, m_additionalInf);
	}
}


// Reset the current character
void CharacterCreationWidget::resetCharacter()
{
	// Reset all displayed values
	nameEdit->setText("");
	initiativeBox->setValue(0);
	initModifierBox->setValue(0);
	isNPCBox->setChecked(false);
	hpBox->setValue(0);
	additionalInformationEdit->setText("");
	// Reset all underlying values
	m_name = "";
	m_initiative = 0;
	m_modifier = 0;
	m_isNPC = false;
	m_hp = 0;
	m_additionalInf = "";
	this->update();
}


void CharacterCreationWidget::createWidget()
{
	bottomFiller = new QWidget;
	bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	// First, allocate all layouts
	// The characterCreationLayout will be displayed, so it gets the instance of this class
	characterCreationLayout = new QVBoxLayout(this);
	characterInformationLayout = new QHBoxLayout;
	additionalInformationLayout = new QHBoxLayout;
	buttonsLayout = new QHBoxLayout;
	// Now start with the headlining label. Set it's properties and align it to the highest and left position
	headlineLabel = new QLabel("Please enter the character stats.");
	QFont f(f.defaultFamily(), 12, QFont::DemiBold);

	headlineLabel->setFont(f);
	headlineLabel->setFrameStyle(QFrame::NoFrame);
	headlineLabel->setAlignment(Qt::AlignLeft);
	headlineLabel->setAlignment(Qt::AlignTop);

	// Now set the text labels, line editing, spin and check boxes for the character information layout
	nameLabel = new QLabel("Name:");
	nameLabel->setFrameStyle(QFrame::NoFrame);
	nameLabel->setAlignment(Qt::AlignLeft);
	nameLabel->setToolTip("Set the name of the character.");
	nameEdit = new QLineEdit;
	initiativeLabel = new QLabel("Initiative:");
	initiativeLabel->setFrameStyle(QFrame::NoFrame);
	initiativeLabel->setAlignment(Qt::AlignLeft);
	initiativeLabel->setToolTip("Set the initiative. This includes ALL modifiers.");
	initiativeBox = new QSpinBox;
	// Cap the lowest initiative at -20.
	// No char will ever have such a bad initiative but we can never know ;-) )
	initiativeBox->setMinimum(-20);
	initModifierLabel = new QLabel("Init modifier:");
	initModifierLabel->setFrameStyle(QFrame::NoFrame);
	initModifierLabel->setAlignment(Qt::AlignLeft);
	initModifierLabel->setToolTip("Set the modificator of the initiative.");
	initModifierBox = new QSpinBox;
	// Same for the modifier and hp, -10 or -100 are really bad but can still be reached
	initModifierBox->setMinimum(-10);
	hpLabel = new QLabel("HP:");
	hpLabel->setFrameStyle(QFrame::NoFrame);
	hpLabel->setAlignment(Qt::AlignLeft);
	hpLabel->setToolTip("Set the HP of this character. This value is optional.");
	hpBox = new QSpinBox;
	hpBox->setRange(-100, 10000);
	isNPCLabel = new QLabel("Is NPC:");
	isNPCLabel->setFrameStyle(QFrame::NoFrame);
	isNPCLabel->setAlignment(Qt::AlignLeft);
	isNPCLabel->setToolTip("Set if this character is an NPC or not.");
	isNPCBox = new QCheckBox;
	// Add all these things to the layout
	characterInformationLayout->addWidget(nameLabel);
	characterInformationLayout->addWidget(nameEdit);
	characterInformationLayout->addWidget(initiativeLabel);
	characterInformationLayout->addWidget(initiativeBox);
	characterInformationLayout->addWidget(initModifierLabel);
	characterInformationLayout->addWidget(initModifierBox);
	characterInformationLayout->addWidget(hpLabel);
	characterInformationLayout->addWidget(hpBox);
	characterInformationLayout->addWidget(isNPCLabel);
	characterInformationLayout->addWidget(isNPCBox);
	characterInformationLayout->setAlignment(Qt::AlignTop);
	// Now another edit for addiotional informations is created
	additionalInformationLabel = new QLabel("Additional information:");
	additionalInformationLabel->setFrameStyle(QFrame::NoFrame);
	additionalInformationLabel->setAlignment(Qt::AlignLeft);
	additionalInformationLabel->setToolTip("Set some additional information. This is optional.");
	additionalInformationEdit = new QLineEdit;
	// Add to layout
	additionalInformationLayout->addWidget(additionalInformationLabel);
	additionalInformationLayout->addWidget(additionalInformationEdit);
	additionalInformationLayout->setAlignment(Qt::AlignTop);

	// Now handle the buttons at the lower end
	saveAnotherButton = new QPushButton("Save and create new character");
	saveAnotherButton->setFixedWidth(200);
	saveAnotherButton->setToolTip("Save this character and create another one.");
	finishButton = new QPushButton("Save and finish");
	finishButton->setFixedWidth(110);
	finishButton->setToolTip("Finish the character creation. Once you're finished, you can't go back.");
	resetButton = new QPushButton("Reset");
	resetButton->setFixedWidth(60);
	resetButton->setToolTip("Reset the current character.");
	cancelButton = new QPushButton("Cancel");
	cancelButton->setFixedWidth(60);
	cancelButton->setToolTip("Cancel the entire character creation. All changes will be lost.");

	// Align the buttons to the right side using a spacer
	QWidget *spacer = new QWidget();

	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	// Add everything to the layout
	buttonsLayout->addWidget(spacer);
	buttonsLayout->addWidget(saveAnotherButton);
	buttonsLayout->addWidget(finishButton);
	buttonsLayout->addWidget(resetButton);
	buttonsLayout->addWidget(cancelButton);

	// Noow set everything to the final layouts
	characterCreationLayout->setContentsMargins(5, 5, 5, 5);
	characterCreationLayout->addSpacing(10);
	// Start with the headlining label
	characterCreationLayout->addWidget(headlineLabel);
	characterCreationLayout->addSpacing(30);
	// Then all layouts
	characterCreationLayout->addLayout(characterInformationLayout);
	characterCreationLayout->addSpacing(10);
	characterCreationLayout->addLayout(additionalInformationLayout);
	characterCreationLayout->addSpacing(10);
	characterCreationLayout->addLayout(buttonsLayout);
	// At the end, create some more space using a filler widget
	characterCreationLayout->addWidget(bottomFiller);
}


// Connect all widgets to the functions
void CharacterCreationWidget::connectWidgets()
{
	connect(nameEdit, SIGNAL(textEdited(const QString&)), this, SLOT(setName(QString)));
	connect(initiativeBox, SIGNAL(valueChanged(int)), this, SLOT(setInitiative(int)));
	connect(initModifierBox, SIGNAL(valueChanged(int)), this, SLOT(setModifier(int)));
	connect(hpBox, SIGNAL(valueChanged(int)), this, SLOT(setHP(int)));
	connect(isNPCBox, SIGNAL(clicked(bool)), this, SLOT(setIsNPC(bool)));
	connect(additionalInformationEdit, SIGNAL(textEdited(const QString&)), this, SLOT(setAdditionalInformation(QString)));

	connect(saveAnotherButton, SIGNAL(clicked()), this, SLOT(saveAndCreateNewCharacter()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(resetCharacter()));
}
