#include "../include/CharacterHandler.hpp"

// Set all values
CharacterHandler::CharacterHandler()
{
}


// Stores a new character in the vector
void
CharacterHandler::storeCharacter(
	std::string	name,
	int		initiative = 0,
	int		modifier = 0,
	bool		isNPC = false,
	int		hp = 0,
	std::string	additionalInf = "")
{
	// Create the shared ptr instance
	characters.push_back(std::make_shared<Character>(Character { name, initiative, modifier, isNPC, hp, additionalInf }));
}


// Sort all created characters according to initiative and modifiers.
void
CharacterHandler::sortCharacters()
{
	// First, use a lambda to sort the characters by their final initiative in descending order
	std::sort(characters.begin(), characters.end(), [](const std::shared_ptr<Character> c1, const std::shared_ptr<Character> c2) {
		return c1->initiative > c2->initiative;
	});

	// Now sort for a second time. This time characters with equal initiatives are sorted using their modificators
	std::sort(characters.begin(), characters.end(), [](const std::shared_ptr<Character> c1, const std::shared_ptr<Character> c2) {
		// If the initiative is equal, the character with a higher modificator comes first
		if (c1->initiative == c2->initiative) {
			return c1->modifier > c2->modifier;
		}
		return false;
	});

	// Last sort. If the initiative and the modifier are equal, the char with the higher address value is taken
	// Thus, we randomize the characters a bit
	// This replaces the typical reroll in this case
	std::sort(characters.begin(), characters.end(), [](const std::shared_ptr<Character> c1, const std::shared_ptr<Character> c2) {
		if (c1->initiative == c2->initiative && c1->modifier == c2->modifier) {
			return c1.get() > c2.get();
		}
		return false;
	});
}


// Remove all characters if the user cancels during character creation
void
CharacterHandler::clearCharacters()
{
	// Clear if not empty
	if (!characters.empty()) {
		characters.clear();
	}
}
