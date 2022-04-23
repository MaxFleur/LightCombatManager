#include "CharacterHandler.hpp"

#include <QCryptographicHash>

// Stores a new character in the vector
void
CharacterHandler::storeCharacter(
	QString name,
	int	initiative,
	int	modifier,
	int	hp,
	bool	isEnemy,
	QString additionalInf)
{
	characters.push_back(std::make_shared<Character>(
				     Character { name, initiative, modifier, hp, isEnemy, additionalInf }
				     ));
}


// Sort all created characters
void
CharacterHandler::sortCharacters(MainSettings::Ruleset ruleset, bool rollAutomatically)
{
	std::sort(characters.begin(), characters.end(),
		  [ruleset, rollAutomatically](const std::shared_ptr<Character> c1, const std::shared_ptr<Character> c2) {
		// Common for all rulesets: Sort for higher initiative
		if (c1->initiative != c2->initiative) {
			return c1->initiative > c2->initiative;
		} else {
			switch (ruleset) {
				// PF 1E/D&D 3.5 rules: Sort for higher INI mod
				// If that's equal, sort automatically or let the party decide
				case MainSettings::Ruleset::PATHFINDER_1E_DND_3_5E:
					if (c1->modifier != c2->modifier) {
						return c1->modifier > c2->modifier;
					} else {
						if (rollAutomatically) {
							return QCryptographicHash::hash(c1->name.toUtf8(), QCryptographicHash::Sha256).toHex() >
							       QCryptographicHash::hash(c2->name.toUtf8(), QCryptographicHash::Sha256).toHex();
						}
					}
				// PF 2E rules: If there is a tie between player and foe, foe goes first
				// Otherwise sort automatically or let the party decide
				case MainSettings::Ruleset::PATHFINDER_2E:
					if (c1->isEnemy != c2->isEnemy) {
						return c1->isEnemy > c2->isEnemy;
					} else {
						if (rollAutomatically) {
							return QCryptographicHash::hash(c1->name.toUtf8(), QCryptographicHash::Sha256).toHex() >
							       QCryptographicHash::hash(c2->name.toUtf8(), QCryptographicHash::Sha256).toHex();
						}
					}
				// D&D 5E rules: Just sort automatically or let the party decide
				case MainSettings::Ruleset::DND_5E:
					if (rollAutomatically) {
						return QCryptographicHash::hash(c1->name.toUtf8(), QCryptographicHash::Sha256).toHex() >
						       QCryptographicHash::hash(c2->name.toUtf8(), QCryptographicHash::Sha256).toHex();
					}
			}
		}
		return false;
	});
}


void
CharacterHandler::clearCharacters()
{
	if (!characters.empty()) {
		characters.clear();
	}
}
