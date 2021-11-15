#include "CharacterHandler.hpp"

#include <QCryptographicHash>

// Set all values
CharacterHandler::CharacterHandler()
{
}


// Stores a new character in the vector
void
CharacterHandler::storeCharacter(
	QString name,
	int	initiative,
	int	modifier,
	int	hp,
	bool	isNPC,
	QString additionalInf)
{
	characters.push_back(
		std::make_shared<Character>(
			Character { name, initiative, modifier, hp, isNPC,
				    additionalInf }));
}


// Sort all created characters. The rules for sorting are defined in the Pathfinder-1E-rulebook.
void
CharacterHandler::sortCharacters()
{
	std::sort(
		characters.begin(),
		characters.end(),
		[](const std::shared_ptr<Character> c1, const std::shared_ptr<Character> c2) {
			// Sort for higher initiative
			if (c1->initiative != c2->initiative) {
				return c1->initiative > c2->initiative;
			} else {
				// If initiative is equal, sort for the higher modificator
				if (c1->modifier != c2->modifier) {
					return c1->modifier > c2->modifier;
					// If initiative and modifiers are equal, sort by the name hashes
				} else {
					return QCryptographicHash::hash(
						c1->name.toUtf8(),
						QCryptographicHash::Sha256).toHex() >
					       QCryptographicHash::hash(
						c2->name.toUtf8(),
						QCryptographicHash::Sha256).toHex();
				}
			}
			return false;
		}
		);
}


void
CharacterHandler::clearCharacters()
{
	if (!characters.empty()) {
		characters.clear();
	}
}
