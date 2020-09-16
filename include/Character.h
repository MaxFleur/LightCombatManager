#pragma once

#include <string>
#include <memory>

// This class represents a character that is created
class Character {
    
public:
    Character(std::string name, int initiative, int modifier, bool isNPC, int hp = 0,std::string additionalInf = "");
    
    // Reset the complete character stats
    void resetCharacter();
    
private:
    // Name of the character
    std::string m_name;
    // Rolled initiave, including all modifiers
    int         m_initiative;
    // The initiative modifier
    int         m_modifier;
    // Is the character an NPC or not
    bool        m_isNPC;
    // The characters hp
    int         m_hp;
    // Optional additional information about a character
    std::string m_additionalInf;
};
using CharacterRef = std::shared_ptr<Character>;
