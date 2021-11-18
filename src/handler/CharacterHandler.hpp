#pragma once

#include <memory>
#include <vector>

#include <QString>

// This class handles the creation, sorting and deletion of the created characters
class CharacterHandler {
private:

	// Main character instance used for the table generation
	struct Character {
		// Name of the character. Because this name must be set manually, it is the only variable without a standard value
		QString name;
		// Final initiative roll of the character, including all modifiers
		int	initiative = 0;
		// Modifiers only
		int	modifier = 0;
		// The hp for this character. Optional.
		int	hp = 0;
		// Is the character an NPC or not
		bool	isNPC = false;
		// Additional information (e.g. status effects). Optional.
		QString additionalInf = QString();
	};

	// Vector storing all created characters
	std::vector<std::shared_ptr<Character> > characters;

public:
	CharacterHandler();

	// Store a new character
	void
	storeCharacter(QString	name,
		       int	initiative = 0,
		       int	modifier = 0,
		       int	hp = 0,
		       bool	isNPC = false,
		       QString	additionalInf = QString());

	// Sort the stored characters
	void
	sortCharacters();

	// Clear all characters
	void
	clearCharacters();

	std::vector<std::shared_ptr<Character> >
	getCharacters()
	{
		return characters;
	}
};
using CharacterHandlerRef = std::shared_ptr<CharacterHandler>;