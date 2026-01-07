#pragma once

#include "AdditionalInfoData.hpp"
#include "RuleSettings.hpp"

// This class handles the creation, sorting and deletion of the created characters
class CharacterHandler {
public:
    // Main character instance used for the table generation
    struct Character {
        Character() = default;

        // Name must be set manually, it is the only variable without a standard value
        QString            name;
        // Final initiative roll, including all modifiers
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

    enum class CharacterStatType {
        INITIATIVE,
        HP,
        MAIN_INFO_TEXT
    };

public:
    void
    sortCharacters(const RuleSettings::Ruleset& ruleset,
                   bool                         rollAutomatically);

    void
    swapCharacters(int i,
                   int j)
    {
        m_characters.swapItemsAt(i, j);
    }

    void
    rotateCharacters(int oldIndex,
                     int newIndex)
    {
        if (oldIndex > newIndex) {
            std::rotate(m_characters.rend() - oldIndex - 1, m_characters.rend() - oldIndex, m_characters.rend() - newIndex);
        } else {
            std::rotate(m_characters.begin() + oldIndex, m_characters.begin() + oldIndex + 1, m_characters.begin() + newIndex + 1);
        }
    }

    void
    clearCharacters()
    {
        if (!m_characters.empty()) {
            m_characters.clear();
        }
    }

    void
    storeCharacter(QString            name,
                   int                initiative,
                   int                modifier,
                   int                hp,
                   bool               isEnemy,
                   AdditionalInfoData additionalInfoData)
    {
        m_characters.emplace_back(Character(name, initiative, modifier, hp, isEnemy, additionalInfoData));
    }

    void
    duplicateCharacter(int index)
    {
        m_characters.insert(index + 1, CharacterHandler::Character(m_characters.at(index)));
    }

    void
    removeCharacter(int index)
    {
        m_characters.remove(index);
    }

    template<CharacterStatType statType>
    void
    setCharacterSingleStat(const QVariant& variant,
                           int             index)
    {
        if constexpr (statType == CharacterStatType::INITIATIVE) {
            m_characters[index].initiative = variant.toInt();
        } else if constexpr (statType == CharacterStatType::HP) {
            m_characters[index].hp = std::clamp(m_characters[index].hp + variant.toInt(), -10000, 10000);
        } else {
            m_characters[index].additionalInfoData.mainInfoText += " " + variant.toString();
        }
    }

    QVector<AdditionalInfoData::StatusEffect>&
    getCharacterStatusEffect(int index)
    {
        return m_characters[index].additionalInfoData.statusEffects;
    }

    const Character&
    characterAt(int index)
    {
        return m_characters.at(index);
    }

    [[nodiscard]] int
    getNumberOfCharacters() const
    {
        return m_characters.size();
    }

private:
    // Vector storing all created characters
    QVector<Character> m_characters;
};

Q_DECLARE_METATYPE(CharacterHandler::Character);
