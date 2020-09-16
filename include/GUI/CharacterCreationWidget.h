#pragma once

#include <QtWidgets>

// This class handles the widget used for the creation of characters
class CharacterCreationWidget : public QWidget {
    
     
public:
    CharacterCreationWidget(QWidget *parent = 0);
    
private:
    void createWidget();
    // Widget for spacing, layouts for the input options
    QWidget     *bottomFiller;
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
