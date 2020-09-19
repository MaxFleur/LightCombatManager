#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>

// This class handles the creation, sorting and deletion of the created characters
class CharacterHandler {
    
public:
    CharacterHandler();
    
    // Store a new character
    void storeCharacter(std::string name,
                        int initiative,
                        int modifier,
                        bool isNPC,
                        int,
                        std::string additionalInf);
    
    // Clear all characters
    void clearCharacters();
    
private:
    // The struct for the character
    struct Character {
        // Name of the character. Because this name must be set manually by all means, it is the only variable without a standard value
        std::string name;
        // Final initiative roll of the character, including all modifiers
        int         initiative = 0;
        // Modifiers only
        int         modifier = 0;
        // Is the character an NPC or not
        bool        isNPC = false;
        // The hp for this character (not necessarily needed)
        int         hp = 0;
        // Additional information (e.g. status effects). Optional.
        std::string additionalInf = "";
    };
    
    // Vector storing all created characters
    std::vector<std::shared_ptr<Character>> characters;
};
using CharacterHandlerRef = std::shared_ptr<CharacterHandler>;
