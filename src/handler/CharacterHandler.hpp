#pragma once

#include "AdditionalInfoData.hpp"
#include "RuleSettings.hpp"

// This class handles the creation, sorting and deletion of the created characters
class CharacterHandler {
public:
    // Main character instance used for the table generation
    struct Character {
        // Name of the character. Because this name must be set manually, it is the only variable without a standard value
        QString            name;
        // Final initiative roll of the character, including all modifiers
        int                initiative;
        // Modifiers only
        int                modifier;
        // The hp for this character
        int                hp;
        // Is the character an enemy or not
        bool               isEnemy;
        // Various information, including status effects
        AdditionalInfoData additionalInfoData;

        Character(const QString& name, int initiative, int modifier, int hp, bool isEnemy,
                  const AdditionalInfoData& additionalInfoData) :
            name(std::move(name)), initiative(std::move(initiative)),
            modifier(std::move(modifier)), hp(std::move(hp)), isEnemy(std::move(isEnemy)),
            additionalInfoData(std::move(additionalInfoData))
        {
        }
    };

public:
    void
    storeCharacter(QString            name,
                   int                initiative,
                   int                modifier,
                   int                hp,
                   bool               isEnemy,
                   AdditionalInfoData additionalInfoData);

    void
    sortCharacters(const RuleSettings::Ruleset& ruleset,
                   bool                         rollAutomatically);

    void
    clearCharacters();

    [[nodiscard]] QVector<Character>&
    getCharacters()
    {
        return characters;
    }

private:
    // Vector storing all created characters
    QVector<Character> characters;
};
