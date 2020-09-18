#pragma once

#include <QtWidgets>
#include "../CharacterSort.h"

// This class handles the widget used for the creation of characters
class CharacterCreationWidget : public QWidget {
    Q_OBJECT
     
public:
    CharacterCreationWidget(QWidget *parent = 0);
    
private slots:
    void setName(const QString &name);
    void setInitiative(int initiative);
    void setModifier(int modifier);
    void setIsNPC(bool isNPC);
    void setHP(int hp);
    void setAdditionalInformation(const QString &additionalInf);
    
    void saveAndCreateNewCharacter();
    void finishCreation();
    void resetCharacter();
    void cancel();
    
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
    
    // Check if the name has been set
    bool isNameSet;
    
    // The values needed for the creation of another character
    std::string m_name;
    int         m_initiative;
    int         m_modifier;
    bool        m_isNPC;
    int         m_hp;
    std::string m_additionalInf;
    
    CharacterSortRef m_charSort;
};
