#include "../../include/GUI/CharacterCreationLayout.h"

// Create the layout at instantiation
CharacterCreationLayout::CharacterCreationLayout() {
    createLayout();
}

void CharacterCreationLayout::createLayout() {
    
    bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // First, allocate all layouts
    characterCreationLayout = new QVBoxLayout;
    characterInformationLayout = new QHBoxLayout;
    additionalInformationLayout = new QHBoxLayout;
    buttonsLayout = new QHBoxLayout;
    // Now start with the headlining label. Set it's properties and align it to the highest and left position
    headlineLabel = new QLabel("Please enter the character stats.");
    QFont f("Arial", 12, QFont::Bold);
    headlineLabel->setFont(f);
    headlineLabel->setFrameStyle(QFrame::NoFrame);
    headlineLabel->setAlignment(Qt::AlignLeft);
    headlineLabel->setAlignment(Qt::AlignTop);
    
    // Now set the text labels, line editing, spin and check boxes for the character information layout
    nameLabel = new QLabel("Name:");
    nameLabel->setFrameStyle(QFrame::NoFrame);
    nameLabel->setAlignment(Qt::AlignLeft);
    nameEdit = new QLineEdit;
    initiativeLabel = new QLabel("Initiative:");
    initiativeLabel->setFrameStyle(QFrame::NoFrame);
    initiativeLabel->setAlignment(Qt::AlignLeft);
    initiativeBox = new QSpinBox;
    // Cap the lowest initiative at -20.
    // No char will ever have such a bad initiative but we can never know ;-) )
    initiativeBox->setMinimum(-20);
    initModifierLabel = new QLabel("Init modifier:");
    initModifierLabel->setFrameStyle(QFrame::NoFrame);
    initModifierLabel->setAlignment(Qt::AlignLeft);
    initModifierBox = new QSpinBox;
    // Same for the modifier and hp, -10 or -100 are really bad but can still be reached
    initModifierBox->setMinimum(-10);
    hpLabel = new QLabel("HP:");
    hpLabel->setFrameStyle(QFrame::NoFrame);
    hpLabel->setAlignment(Qt::AlignLeft);
    hpBox = new QSpinBox;
    hpBox->setMinimum(-100);
    isNPCLabel = new QLabel("Is NPC:");
    isNPCLabel->setFrameStyle(QFrame::NoFrame);
    isNPCLabel->setAlignment(Qt::AlignLeft);
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
    additionalInformationEdit = new QLineEdit;
    // Add to layout
    additionalInformationLayout->addWidget(additionalInformationLabel);
    additionalInformationLayout->addWidget(additionalInformationEdit);
    additionalInformationLayout->setAlignment(Qt::AlignTop);
    
    // Now handle the buttons at the lower end
    saveAnotherButton = new QPushButton("Save and create new character");
    finishButton = new QPushButton("Finish character creation");
    resetButton = new QPushButton("Reset");QWidget *
    cancelButton = new QPushButton("Cancel");
    // Set an own width for every button
    saveAnotherButton->setFixedWidth(200);
    finishButton->setFixedWidth(180);
    resetButton->setFixedWidth(60);
    cancelButton->setFixedWidth(60);
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
