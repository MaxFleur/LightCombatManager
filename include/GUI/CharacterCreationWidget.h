#pragma once

#include <QtWidgets>
#include "../CharacterHandler.h"
#include <iostream>

// This class handles the widget used for the creation of characters
class CharacterCreationWidget : public QWidget {
    Q_OBJECT
     
public:
    CharacterCreationWidget(CharacterHandlerRef charSort, QWidget *parent = 0);
    // Return the cancel button, connecting the parent widget to this button
    QPushButton* getCancelButton() { return cancelButton; }
    QPushButton* getFinishButton() { return finishButton; }
    
private slots:
    void setName(const QString &name);
    void setInitiative(int initiative);
    void setModifier(int modifier);
    void setIsNPC(bool isNPC);
    void setHP(int hp);
    void setAdditionalInformation(const QString &additionalInf);
    
    void saveAndCreateNewCharacter();
    void resetCharacter();
    
private:
    // Create the widget
    void createWidget();
    void connectWidgets();
    
    // All GUI elements
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
    
    // The values needed for the creation of another character
    std::string m_name;
    int         m_initiative;
    int         m_modifier;
    bool        m_isNPC;
    int         m_hp;
    std::string m_additionalInf;
    // character sort object to store newly created objects
    CharacterHandlerRef m_char;
};
