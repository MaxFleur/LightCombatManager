#include "CharacterHandler.hpp"

#include <QCryptographicHash>

// Stores a new character
void
CharacterHandler::storeCharacter(
    QString                                   name,
    int                                       initiative,
    int                                       modifier,
    int                                       hp,
    bool                                      isEnemy,
    AdditionalInfoData::AdditionalInformation additionalInformation)
{
    characters.push_back(Character { name, initiative, modifier, hp, isEnemy, additionalInformation });
}


// Sort all created characters, depending on the used rulset
void
CharacterHandler::sortCharacters(const RuleSettings::Ruleset& ruleset, bool rollAutomatically)
{
    const auto sortUsingHashes = [&] (const auto& c1, const auto& c2) {
        // Use the ini value as additional seed. This will deliver different results depending on
        // the ini value, but stay constant if the table is stored and reopened
        const auto nameToHashOne = c1.name + " " + QString::number(c1.initiative);
        const auto nameToHashTwo = c2.name + " " + QString::number(c2.initiative);
        return rollAutomatically ?
               QCryptographicHash::hash(nameToHashOne.toUtf8(), QCryptographicHash::Sha256).toHex() >
               QCryptographicHash::hash(nameToHashTwo.toUtf8(), QCryptographicHash::Sha256).toHex() :
               false;
    };

    std::sort(characters.begin(), characters.end(),
              [ruleset, rollAutomatically, sortUsingHashes](const auto& c1, const auto& c2) {
        // Common for all rulesets: Sort for higher initiative
        if (c1.initiative != c2.initiative) {
            return c1.initiative > c2.initiative;
        }
        switch (ruleset) {
        // PF 1E/D&D 3.5/Starfinder rules: Sort for higher INI mod
        // If that's equal, sort automatically or let the party decide
        case RuleSettings::Ruleset::PATHFINDER_1E_DND_35E:
        case RuleSettings::Ruleset::STARFINDER:
        // D&D 3.0 uses the dex value for ties, but this is essentially another variant
        // of the mod value, so no additional changes are necessary
        case RuleSettings::Ruleset::DND_30E:
            if (c1.modifier != c2.modifier) {
                return c1.modifier > c2.modifier;
            }
            return sortUsingHashes(c1, c2);
        // PF 2E rules: If there is a tie between player and foe, foe goes first
        // Otherwise sort automatically or let the party decide
        case RuleSettings::Ruleset::PATHFINDER_2E:
            if (c1.isEnemy != c2.isEnemy) {
                return c1.isEnemy > c2.isEnemy;
            }
            return sortUsingHashes(c1, c2);
        // D&D 5E rules: Just sort automatically or let the party decide
        case RuleSettings::Ruleset::DND_5E:
            return sortUsingHashes(c1, c2);
        default:
            return false;
        }
    });
}


void
CharacterHandler::clearCharacters()
{
    if (!characters.empty()) {
        characters.clear();
    }
}
