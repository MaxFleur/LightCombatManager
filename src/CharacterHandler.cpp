#include "../include/CharacterHandler.h"

// Set all values
CharacterHandler::CharacterHandler() {
    // Temporary test values 
    characters.push_back(std::make_shared<Character>( Character { "Player1", 11, -1, false, 0, "" }));
    characters.push_back(std::make_shared<Character>( Character { "Player3", 11, -1, false, 0, "NOTE1" }));
    characters.push_back(std::make_shared<Character>( Character { "Anti-Paladin", 11, -1, true, 0, "NOTE2" }));
    characters.push_back(std::make_shared<Character>( Character { "Ghoul", 11, -1, true, 0, "NOTE3" }));
    characters.push_back(std::make_shared<Character>( Character { "Goblin1", 20, 7, true, 0, "INI + 4" }));
    characters.push_back(std::make_shared<Character>( Character { "Player4", 20, 7, false, 0, "Haste" }));
    characters.push_back(std::make_shared<Character>( Character { "Player2", 20, 7, false, 0, "Stricken" }));
    characters.push_back(std::make_shared<Character>( Character { "Necromant", 20, 7, true, 0, "Dazed" }));
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

// Sort all created characters according to initiative and modifiers.
void CharacterHandler::sortCharacters() {
    // First, use a lambda to sort the characters by their final initiative in descending order
    std::sort(characters.begin(), characters.end(), [](const std::shared_ptr<Character> c1, const std::shared_ptr<Character> c2) {
        return c1->initiative > c2->initiative;
    });
    
    // Now sort for a second time. This time characters with equal initiatives are sorted using their modificators
    std::sort(characters.begin(), characters.end(), [](const std::shared_ptr<Character> c1, const std::shared_ptr<Character> c2) {
        // If the initiative is equal, the character with a higher modificator comes first
        if(c1->initiative == c2->initiative) {
            return c1->modifier > c2->modifier; 
        } return false;
    });
    
    // Right now, print the sorted characters for testing purposes
    for(int i = 0; i < characters.size(); i++) {
        std::cout << characters.at(i)->name << " " << characters.at(i)->initiative << " " << characters.at(i)->modifier << " " << characters.at(i)->additionalInf << std::endl; 
    }
}

// Remove all characters if the user cancels during character creation
void CharacterHandler::clearCharacters() {
    // Clear if not empty
    if(!characters.empty()) {
        characters.clear();
    }
}
