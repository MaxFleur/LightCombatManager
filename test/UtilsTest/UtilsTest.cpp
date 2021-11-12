#include <QString>
#include <QTableWidget>

#include <catch2/catch.hpp>

#include "Utils.hpp"


TEST_CASE("Util Testing", "[Utils]"){
	auto *const tableWidget = new QTableWidget(1, 6);
	tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
	tableWidget->setItem(0, 1, new QTableWidgetItem("19"));
	tableWidget->setItem(0, 2, new QTableWidgetItem("2"));
	tableWidget->setItem(0, 3, new QTableWidgetItem("36"));
	tableWidget->setItem(0, 4, new QTableWidgetItem(""));
	tableWidget->setItem(0, 5, new QTableWidgetItem("Haste"));

	SECTION("Resynchronizing characters test") {
		std::shared_ptr<CharacterHandler> charHandler = std::make_shared<CharacterHandler>();
		Utils::resynchronizeCharacters(tableWidget, charHandler);

		SECTION("Check name") {
			REQUIRE(charHandler->getCharacters().at(0)->name == "Fighter");
		}
		SECTION("Check INI") {
			REQUIRE(charHandler->getCharacters().at(0)->initiative == 19);
		}
		SECTION("Check modifier") {
			REQUIRE(charHandler->getCharacters().at(0)->modifier == 2);
		}
		SECTION("Check hp") {
			REQUIRE(charHandler->getCharacters().at(0)->hp == 36);
		}
		SECTION("Check empty npc field") {
			REQUIRE(charHandler->getCharacters().at(0)->isNPC == false);
		}
		SECTION("Check filled NPC field") {
			tableWidget->setItem(0, 4, new QTableWidgetItem("X"));
			Utils::resynchronizeCharacters(tableWidget, charHandler);
			REQUIRE(charHandler->getCharacters().at(0)->isNPC == true);
		}
		SECTION("Additional information") {
			REQUIRE(charHandler->getCharacters().at(0)->additionalInf == "Haste");
		}
	}

	SECTION("Semicolon test") {
		SECTION("Contains semicolon in first column") {
			tableWidget->setItem(0, 0, new QTableWidgetItem("Figh;ter"));
			REQUIRE(Utils::containsSemicolon(tableWidget));
		}
		SECTION("Contains semicolon in sixth column") {
			tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
			tableWidget->setItem(0, 5, new QTableWidgetItem("Has;te"));
			REQUIRE(Utils::containsSemicolon(tableWidget));
		}
		SECTION("Contains no semicolon") {
			tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
			tableWidget->setItem(0, 5, new QTableWidgetItem("Haste"));
			REQUIRE(!Utils::containsSemicolon(tableWidget));
		}
	}
}
