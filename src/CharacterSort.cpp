#include "../include/CharacterSort.h"

// Set all values
CharacterSort::CharacterSort() {
        
} 

// Remove all characters if the user cancels during character creation
void CharacterSort::clearCharacters() {
    // Clear if not empty
    if(!characters.empty()) {
        characters.clear();
    }
}
