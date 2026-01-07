#include "AdditionalInfoData.hpp"
#include "CharacterHandler.hpp"
#include "RuleSettings.hpp"

#ifdef CATCH2_V3
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include <memory>

TEST_CASE("CharacterHandler Testing", "[CharacterHandler]") {
    auto const charHandler = std::make_shared<CharacterHandler>();

    SECTION("Sorting tests") {
        RuleSettings ruleSettings;

        SECTION("Sorting test - PF1/D&D 3.5/D&D 3.0/Starfinder") {
            ruleSettings.ruleset = RuleSettings::Ruleset::PATHFINDER_1E_DND_35E;

            charHandler->storeCharacter("Bard", 12, 2, 29, false, {});
            charHandler->storeCharacter("Zombie", 12, 1, 13, true, {});
            charHandler->storeCharacter("Fighter", 19, 4, 36, false, {});
            charHandler->storeCharacter("Undead Boss", 23, 6, 62, true, {});
            charHandler->storeCharacter("Cleric", 7, 1, 31, false, {});
            charHandler->storeCharacter("Ranger", 27, 8, 36, false, {});

            charHandler->sortCharacters(ruleSettings.ruleset, ruleSettings.rollAutomatical);
            REQUIRE(charHandler->characterAt(0).name == "Ranger");
            REQUIRE(charHandler->characterAt(1).name == "Undead Boss");
            REQUIRE(charHandler->characterAt(2).name == "Fighter");
            REQUIRE(charHandler->characterAt(3).name == "Bard");
            REQUIRE(charHandler->characterAt(4).name == "Zombie");
            REQUIRE(charHandler->characterAt(5).name == "Cleric");
        }
        SECTION("Sorting test - PF2") {
            ruleSettings.ruleset = RuleSettings::Ruleset::PATHFINDER_2E;

            charHandler->storeCharacter("Bard", 12, 2, 29, false, {});
            charHandler->storeCharacter("Zombie", 12, 1, 13, true, {});
            charHandler->storeCharacter("Fighter", 12, 4, 36, false, {});
            charHandler->storeCharacter("Undead Boss", 12, 6, 62, true, {});
            charHandler->storeCharacter("Cleric", 7, 1, 31, false, {});
            charHandler->storeCharacter("Ranger", 27, 8, 36, false, {});

            charHandler->sortCharacters(ruleSettings.ruleset, ruleSettings.rollAutomatical);
            REQUIRE(charHandler->characterAt(0).name == "Ranger");
            REQUIRE(charHandler->characterAt(1).name == "Zombie");
            REQUIRE(charHandler->characterAt(2).name == "Undead Boss");
            REQUIRE(charHandler->characterAt(3).name == "Bard");
            REQUIRE(charHandler->characterAt(4).name == "Fighter");
            REQUIRE(charHandler->characterAt(5).name == "Cleric");
        }
        SECTION("Sorting test - D&D 5E") {
            ruleSettings.ruleset = RuleSettings::Ruleset::DND_5E;

            charHandler->storeCharacter("Bard", 3, 2, 29, false, {});
            charHandler->storeCharacter("Zombie", 14, 1, 13, true, {});
            charHandler->storeCharacter("Fighter", 20, 4, 36, false, {});
            charHandler->storeCharacter("Undead Boss", 20, 6, 62, true, {});
            charHandler->storeCharacter("Cleric", 14, 1, 31, false, {});
            charHandler->storeCharacter("Ranger", 27, 8, 36, false, {});

            charHandler->sortCharacters(ruleSettings.ruleset, ruleSettings.rollAutomatical);
            REQUIRE(charHandler->characterAt(0).name == "Ranger");
            REQUIRE(charHandler->characterAt(1).name == "Fighter");
            REQUIRE(charHandler->characterAt(2).name == "Undead Boss");
            REQUIRE(charHandler->characterAt(3).name == "Zombie");
            REQUIRE(charHandler->characterAt(4).name == "Cleric");
            REQUIRE(charHandler->characterAt(5).name == "Bard");
        }
    }

    SECTION("Storage test") {
        AdditionalInfoData::StatusEffect statusEffect{ "Dazed", false, 2 };
        AdditionalInfoData additionalInfoData { { statusEffect }, "Fire Resistance" };

        charHandler->storeCharacter("Witch", 14, 5, 23, true, additionalInfoData);

        REQUIRE(charHandler->getNumberOfCharacters() == 1);

        REQUIRE(charHandler->characterAt(0).name == "Witch");
        REQUIRE(charHandler->characterAt(0).initiative == 14);
        REQUIRE(charHandler->characterAt(0).modifier == 5);
        REQUIRE(charHandler->characterAt(0).hp == 23);
        REQUIRE(charHandler->characterAt(0).isEnemy == true);
        REQUIRE(charHandler->characterAt(0).additionalInfoData.statusEffects.at(0).name == "Dazed");
        REQUIRE(charHandler->characterAt(0).additionalInfoData.statusEffects.at(0).isPermanent == false);
        REQUIRE(charHandler->characterAt(0).additionalInfoData.statusEffects.at(0).duration == 2);
        REQUIRE(charHandler->characterAt(0).additionalInfoData.mainInfoText == "Fire Resistance");
    }

    charHandler->storeCharacter("Bard", 3, 2, 29, false, {});
    charHandler->storeCharacter("Zombie", 14, 1, 13, true, {});
    charHandler->storeCharacter("Fighter", 20, 4, 36, false, {});
    charHandler->storeCharacter("Undead Boss", 20, 6, 62, true, {});

    SECTION("Swap characters test") {
        charHandler->swapCharacters(1, 2);
        REQUIRE(charHandler->characterAt(1).name == "Fighter");
        REQUIRE(charHandler->characterAt(2).name == "Zombie");
    }
    SECTION("Rotate characters test") {
        SECTION("Old index > new index") {
            charHandler->rotateCharacters(2, 0);
            REQUIRE(charHandler->characterAt(0).name == "Fighter");
            REQUIRE(charHandler->characterAt(1).name == "Bard");
            REQUIRE(charHandler->characterAt(2).name == "Zombie");
            REQUIRE(charHandler->characterAt(3).name == "Undead Boss");
        }
        SECTION("New index > old index") {
            charHandler->rotateCharacters(0, 2);
            REQUIRE(charHandler->characterAt(0).name == "Zombie");
            REQUIRE(charHandler->characterAt(1).name == "Fighter");
            REQUIRE(charHandler->characterAt(2).name == "Bard");
            REQUIRE(charHandler->characterAt(3).name == "Undead Boss");
        }
    }
    SECTION("Clear characters test") {
        charHandler->clearCharacters();
        REQUIRE(charHandler->getNumberOfCharacters() == 0);
    }

    SECTION("Duplicate character test") {
        charHandler->duplicateCharacter(1);

        REQUIRE(charHandler->getNumberOfCharacters() == 5);
        REQUIRE(charHandler->characterAt(2).name == "Zombie");
        REQUIRE(charHandler->characterAt(2).initiative == 14);
        REQUIRE(charHandler->characterAt(2).modifier == 1);
        REQUIRE(charHandler->characterAt(2).hp == 13);
        REQUIRE(charHandler->characterAt(2).isEnemy == true);
    }
    SECTION("Remove character test") {
        charHandler->removeCharacter(1);

        REQUIRE(charHandler->getNumberOfCharacters() == 3);
        REQUIRE(charHandler->characterAt(0).name == "Bard");
        REQUIRE(charHandler->characterAt(1).name == "Fighter");
        REQUIRE(charHandler->characterAt(2).name == "Undead Boss");
    }

    SECTION("Set character single stat test") {
        charHandler->setCharacterSingleStat<CharacterHandler::CharacterStatType::INITIATIVE>(QVariant::fromValue(12), 0);
        REQUIRE(charHandler->characterAt(0).initiative == 12);

        charHandler->setCharacterSingleStat<CharacterHandler::CharacterStatType::HP>(QVariant::fromValue(13), 0);
        REQUIRE(charHandler->characterAt(0).hp == 42);
        charHandler->setCharacterSingleStat<CharacterHandler::CharacterStatType::HP>(QVariant::fromValue(10000), 0);
        REQUIRE(charHandler->characterAt(0).hp == 10000);

        QString testString("Test string");
        charHandler->setCharacterSingleStat<CharacterHandler::CharacterStatType::MAIN_INFO_TEXT>(QVariant::fromValue(testString), 0);
        REQUIRE(charHandler->characterAt(0).additionalInfoData.mainInfoText == " Test string");
    }
}
