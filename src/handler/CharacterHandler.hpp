#pragma once

#include <memory>

#include <QString>
#include <QVector>

#include "RuleSettings.hpp"

// This class handles the creation, sorting and deletion of the created characters
class CharacterHandler {
public:
    // Main character instance used for the table generation
    struct Character {
        // Name of the character. Because this name must be set manually, it is the only variable without a standard value
        QString name;
        // Final initiative roll of the character, including all modifiers
        int     initiative = 0;
        // Modifiers only
        int     modifier = 0;
        // The hp for this character
        int     hp = 0;
        // Is the character an enemy or not
        bool    isEnemy = false;
        // Additional information (e.g. status effects)
        QString additionalInformation = QString();
    };

public:
    void
    storeCharacter(QString name,
                   int     initiative = 0,
                   int     modifier = 0,
                   int     hp = 0,
                   bool    isEnemy = false,
                   QString additionalInf = QString());

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
using CharacterHandlerRef = std::shared_ptr<CharacterHandler>;
