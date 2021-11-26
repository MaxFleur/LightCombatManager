#include <QString>

#include <catch2/catch.hpp>

#include "CharacterHandler.hpp"


TEST_CASE("CharacterHandler Testing", "[CharacterHandler]"){
	SECTION("Full Character stored test") {
		std::shared_ptr<CharacterHandler> charHandler = std::make_shared<CharacterHandler>();
		charHandler->storeCharacter("Witch", 14, 5, 23, true, "Fire Resistance");

		REQUIRE(charHandler->getCharacters().size() == 1);

		REQUIRE(charHandler->getCharacters().at(0)->name == "Witch");
		REQUIRE(charHandler->getCharacters().at(0)->initiative == 14);
		REQUIRE(charHandler->getCharacters().at(0)->modifier == 5);
		REQUIRE(charHandler->getCharacters().at(0)->hp == 23);
		REQUIRE(charHandler->getCharacters().at(0)->isEnemy == true);
		REQUIRE(charHandler->getCharacters().at(0)->additionalInf == "Fire Resistance");
	}
	SECTION("Minimal Character stored test") {
		std::shared_ptr<CharacterHandler> charHandler = std::make_shared<CharacterHandler>();
		charHandler->storeCharacter("Generic Character");

		REQUIRE(charHandler->getCharacters().at(0)->name == "Generic Character");
		REQUIRE(charHandler->getCharacters().at(0)->initiative == 0);
		REQUIRE(charHandler->getCharacters().at(0)->modifier == 0);
		REQUIRE(charHandler->getCharacters().at(0)->hp == 0);
		REQUIRE(charHandler->getCharacters().at(0)->isEnemy == false);
		REQUIRE(charHandler->getCharacters().at(0)->additionalInf == "");
	}
	SECTION("Sort Characters test") {
		std::shared_ptr<CharacterHandler> charHandler = std::make_shared<CharacterHandler>();
		charHandler->storeCharacter("Bard", 12, 2, 29);
		charHandler->storeCharacter("Zombie", 12, 1, 13, true);
		charHandler->storeCharacter("Fighter", 19, 4, 36, false, "Shaken");
		charHandler->storeCharacter("Undead Boss", 23, 6, 62, true, "Haste");
		charHandler->storeCharacter("Cleric", 7, 1, 31, false, "Fascinated");
		charHandler->storeCharacter("Ranger", 27, 8, 36, false, "Invisible");

		charHandler->sortCharacters();
		REQUIRE(charHandler->getCharacters().at(0)->name == "Ranger");
		REQUIRE(charHandler->getCharacters().at(1)->name == "Undead Boss");
		REQUIRE(charHandler->getCharacters().at(2)->name == "Fighter");
		REQUIRE(charHandler->getCharacters().at(3)->name == "Bard");
		REQUIRE(charHandler->getCharacters().at(4)->name == "Zombie");
		REQUIRE(charHandler->getCharacters().at(5)->name == "Cleric");
	}
	SECTION("Clear Characters test") {
		std::shared_ptr<CharacterHandler> charHandler = std::make_shared<CharacterHandler>();
		charHandler->storeCharacter("Bard", 12, 2, 29);
		charHandler->clearCharacters();

		charHandler->sortCharacters();
		REQUIRE(charHandler->getCharacters().size() == 0);
	}
}
