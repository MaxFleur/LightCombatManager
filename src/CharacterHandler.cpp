#include "../include/CharacterHandler.hpp"

// Set all values
CharacterHandler::CharacterHandler()
{
}


// Stores a new character in the vector
void
CharacterHandler::storeCharacter(
	QString name,
	int	initiative = 0,
	int	modifier = 0,
	bool	isNPC = false,
	int	hp = 0,
	QString additionalInf = "")
{
	// Create the shared ptr instance
	characters.push_back(
		std::make_shared<Character>(
			Character { name, initiative, modifier, isNPC, hp,
				    additionalInf }));
}


// Sort all created characters according to initiative and modifiers.
// The rules for sorting are defined in the Pathfinder-1E-rulebook.
void
CharacterHandler::sortCharacters()
{
	// First, use a lambda to sort the characters by their final initiative in descending order
	std::sort(
		characters.begin(),
		characters.end(),
		[](const std::shared_ptr<Character> c1, const std::shared_ptr<Character> c2) {
			// Check if initiative is different
			if (c1->initiative != c2->initiative) {
				return c1->initiative > c2->initiative;
			} else {
				// If so, check if the modifiers are different
				if (c1->modifier != c2->modifier) {
					return c1->modifier > c2->modifier;
					// If initiative and modifiers are equal, randomize by using the instance adresses
				} else {
					return c1.get() > c2.get();
				}
			}
			return false;
		}
		);
}


// Remove all characters if the user cancels during character creation
void
CharacterHandler::clearCharacters()
{
	if (!characters.empty()) {
		characters.clear();
	}
}
