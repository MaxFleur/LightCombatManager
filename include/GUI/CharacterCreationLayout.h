#pragma once

#include <QtWidgets>

// This class handles the layout used for the creation of characters
class CharacterCreationLayout {
    
     
public:
    CharacterCreationLayout();
    // Return the final layout 
    QVBoxLayout *getCharacterCreationLayout() { return characterCreationLayout; }
    
private:
    void createLayout();
    // Widget for spacing, layouts for the input options
    QWidget     *bottomFiller;
    QVBoxLayout *centeredLayout;
    QHBoxLayout *characterInformationLayout;
    QHBoxLayout *additionalInformationLayout;
    QHBoxLayout *buttonsLayout;
    // The final layout which we will return
    QVBoxLayout *characterCreationLayout;
    // The label containing the headline
    QLabel      *headlineLabel;
    // Input stuff used for the normal char values
    QLineEdit   *nameEdit;
    QSpinBox    *initiativeBox;
    QSpinBox    *initModifierBox;
    QSpinBox    *hpBox;
    QCheckBox   *isNPCBox;
    QLabel      *nameLabel;
    QLabel      *initiativeLabel;
    QLabel      *initModifierLabel;
    QLabel      *hpLabel;
    QLabel      *isNPCLabel;
    // A line edit for additional character information
    QLineEdit   *additionalInformationEdit;
    QLabel      *additionalInformationLabel;
    // Push buttons for saving characters, finishing or aborting the character creation or resetting a character
    QPushButton *saveAnotherButton;
    QPushButton *finishButton;
    QPushButton *resetButton;
    QPushButton *cancelButton;
};
// Shared pointer instance for usage in the mainwindow class
using CharacterCreationLayoutRef = std::shared_ptr<CharacterCreationLayout>; 
