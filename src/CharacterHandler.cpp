#include "../include/CharacterHandler.h"

// Set all values
CharacterHandler::CharacterHandler() {
    // Temporary test values 
    characters.push_back(std::make_shared<Character>( Character { "Test1", 11, 3, false, 0, "" }));
    characters.push_back(std::make_shared<Character>( Character { "Test2", 5, 1, false, 0, "" }));
    characters.push_back(std::make_shared<Character>( Character { "Test3", 20, 3, false, 0, "Bull's strength" }));
    characters.push_back(std::make_shared<Character>( Character { "Test4", 11, 5, false, 0, "" }));
    characters.push_back(std::make_shared<Character>( Character { "Test5", 18, 6, false, 0, "" }));
    characters.push_back(std::make_shared<Character>( Character { "Test6", 20, 5, false, 0, "" }));
    characters.push_back(std::make_shared<Character>( Character { "Test7", 20, 7, false, 0, "INI + 4" }));
    characters.push_back(std::make_shared<Character>( Character { "Test7", 20, 7, false, 0, "" }));
    characters.push_back(std::make_shared<Character>( Character { "Test8", 14, -1, false, 0, "" }));
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
