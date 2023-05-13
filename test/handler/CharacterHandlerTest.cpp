#include <catch2/catch.hpp>

#include "CharacterHandler.hpp"
#include "RuleSettings.hpp"


TEST_CASE("CharacterHandler Testing", "[CharacterHandler]") {
    SECTION("Storage test") {
        SECTION("Full Character stored test") {
            auto const charHandler = std::make_shared<CharacterHandler>();
            charHandler->storeCharacter("Witch", 14, 5, 23, true, "Fire Resistance");

            REQUIRE(charHandler->getCharacters().size() == 1);

            REQUIRE(charHandler->getCharacters().at(0).name == "Witch");
            REQUIRE(charHandler->getCharacters().at(0).initiative == 14);
            REQUIRE(charHandler->getCharacters().at(0).modifier == 5);
            REQUIRE(charHandler->getCharacters().at(0).hp == 23);
            REQUIRE(charHandler->getCharacters().at(0).isEnemy == true);
            REQUIRE(charHandler->getCharacters().at(0).additionalInformation == "Fire Resistance");
        }
        SECTION("Minimal Character stored test") {
            auto const charHandler = std::make_shared<CharacterHandler>();
            charHandler->storeCharacter("Generic Character");

            REQUIRE(charHandler->getCharacters().at(0).name == "Generic Character");
            REQUIRE(charHandler->getCharacters().at(0).initiative == 0);
            REQUIRE(charHandler->getCharacters().at(0).modifier == 0);
            REQUIRE(charHandler->getCharacters().at(0).hp == 0);
            REQUIRE(charHandler->getCharacters().at(0).isEnemy == false);
            REQUIRE(charHandler->getCharacters().at(0).additionalInformation == "");
        }
    }
    SECTION("Sorting tests") {
        auto const ruleSettings = std::make_shared<RuleSettings>();

        SECTION("Sorting test - PF1/D&D 3.5/D&D 3.0/Starfinder") {
            ruleSettings->ruleset = RuleSettings::Ruleset::PATHFINDER_1E_DND_35E;
            auto const charHandler = std::make_shared<CharacterHandler>();

            charHandler->storeCharacter("Bard", 12, 2, 29);
            charHandler->storeCharacter("Zombie", 12, 1, 13, true);
            charHandler->storeCharacter("Fighter", 19, 4, 36, false);
            charHandler->storeCharacter("Undead Boss", 23, 6, 62, true);
            charHandler->storeCharacter("Cleric", 7, 1, 31, false);
            charHandler->storeCharacter("Ranger", 27, 8, 36, false);

            charHandler->sortCharacters(ruleSettings->ruleset, ruleSettings->rollAutomatical);
            REQUIRE(charHandler->getCharacters().at(0).name == "Ranger");
            REQUIRE(charHandler->getCharacters().at(1).name == "Undead Boss");
            REQUIRE(charHandler->getCharacters().at(2).name == "Fighter");
            REQUIRE(charHandler->getCharacters().at(3).name == "Bard");
            REQUIRE(charHandler->getCharacters().at(4).name == "Zombie");
            REQUIRE(charHandler->getCharacters().at(5).name == "Cleric");
        }
        SECTION("Sorting test - PF2") {
            ruleSettings->ruleset = RuleSettings::Ruleset::PATHFINDER_2E;

            auto const charHandler = std::make_shared<CharacterHandler>();

            charHandler->storeCharacter("Bard", 12, 2, 29);
            charHandler->storeCharacter("Zombie", 12, 1, 13, true);
            charHandler->storeCharacter("Fighter", 12, 4, 36, false);
            charHandler->storeCharacter("Undead Boss", 12, 6, 62, true);
            charHandler->storeCharacter("Cleric", 7, 1, 31, false);
            charHandler->storeCharacter("Ranger", 27, 8, 36, false);

            charHandler->sortCharacters(ruleSettings->ruleset, ruleSettings->rollAutomatical);
            REQUIRE(charHandler->getCharacters().at(0).name == "Ranger");
            REQUIRE(charHandler->getCharacters().at(1).name == "Zombie");
            REQUIRE(charHandler->getCharacters().at(2).name == "Undead Boss");
            REQUIRE(charHandler->getCharacters().at(3).name == "Bard");
            REQUIRE(charHandler->getCharacters().at(4).name == "Fighter");
            REQUIRE(charHandler->getCharacters().at(5).name == "Cleric");
        }
        SECTION("Sorting test - D&D 5E") {
            ruleSettings->ruleset = RuleSettings::Ruleset::DND_5E;

            auto const charHandler = std::make_shared<CharacterHandler>();

            charHandler->storeCharacter("Bard", 3, 2, 29);
            charHandler->storeCharacter("Zombie", 14, 1, 13, true);
            charHandler->storeCharacter("Fighter", 20, 4, 36, false);
            charHandler->storeCharacter("Undead Boss", 20, 6, 62, true);
            charHandler->storeCharacter("Cleric", 14, 1, 31, false);
            charHandler->storeCharacter("Ranger", 27, 8, 36, false);

            charHandler->sortCharacters(ruleSettings->ruleset, ruleSettings->rollAutomatical);
            REQUIRE(charHandler->getCharacters().at(0).name == "Ranger");
            REQUIRE(charHandler->getCharacters().at(1).name == "Fighter");
            REQUIRE(charHandler->getCharacters().at(2).name == "Undead Boss");
            REQUIRE(charHandler->getCharacters().at(3).name == "Zombie");
            REQUIRE(charHandler->getCharacters().at(4).name == "Cleric");
            REQUIRE(charHandler->getCharacters().at(5).name == "Bard");
        }
        SECTION("Clear Characters test") {
            auto const charHandler = std::make_shared<CharacterHandler>();

            charHandler->storeCharacter("Bard", 12, 2, 29);
            charHandler->clearCharacters();

            charHandler->sortCharacters(ruleSettings->ruleset, ruleSettings->rollAutomatical);
            REQUIRE(charHandler->getCharacters().size() == 0);
        }
    }
}
