#pragma once

#include <string>
#include <vector>
#include <memory>

// This class handles the sorting of the created characters
class CharacterSort {
    
public:
    CharacterSort();
    
    // Clear all characters
    void clearCharacters();
    
private:
    // The struct for the character
    struct Character {
        // Name of the character. Because this name must be set manually by all means, it is the only variable without a standard value
        std::string m_name;
        // Final initiative roll of the character, including all modifiers
        int         m_initiative = 0;
        // Modifiers only
        int         m_modifier = 0;
        // Is the character an NPC or not
        bool        m_isNPC = false;
        // The hp for this character (not necessarily needed)
        int         m_hp = 0;
        // Additional information (e.g. status effects). Optional.
        std::string m_additionalInf = "";
    };
    
    // Vector storing all created characters
    std::vector<std::shared_ptr<Character>> characters;
};
using CharacterSortRef = std::shared_ptr<CharacterSort>;
