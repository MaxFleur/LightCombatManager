#include "../include/Character.h"

// Set all values
Character::Character(std::string name, int initiative, int modifier, bool isNPC, int hp, std::string additionalInf)
        : m_name(name), m_initiative(initiative), m_modifier(modifier), m_isNPC(isNPC), m_hp(hp), m_additionalInf(additionalInf) {
        
} 

// Reset all values in case the user is not satisfied with this character
void Character::resetCharacter() {
    m_name = "";
    m_initiative = 0;
    m_modifier = 0;
    m_isNPC = false;
    m_hp = 0;
    m_additionalInf = "";
}
