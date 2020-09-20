#include "../include/CharacterHandler.h"

// Set all values
CharacterHandler::CharacterHandler() {
} 

// Stores a new character in the vector
void CharacterHandler::storeCharacter(
                        std::string name,
                        int initiative = 0,
                        int modifier = 0,
                        bool isNPC = false,
                        int hp = 0,
                        std::string additionalInf = 0) {
    // Create the shared ptr instance
    characters.push_back(std::make_shared<Character>( Character { name, initiative, modifier, isNPC, hp, additionalInf }));
}

void CharacterHandler::sortCharacters() {
}

// Remove all characters if the user cancels during character creation
void CharacterHandler::clearCharacters() {
    // Clear if not empty
    if(!characters.empty()) {
        characters.clear();
    }
}
