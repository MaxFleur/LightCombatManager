#include <QCheckBox>
#include <QHBoxLayout>
#include <QString>
#include <QTableWidget>
#include <QWidget>

#include <catch2/catch.hpp>

#include "Utils.hpp"


TEST_CASE("Util Testing", "[Utils]"){
	auto *const tableWidget = new QTableWidget(1, 6);
	auto *const checkBox = new QCheckBox;
	checkBox->setChecked(false);

	// Center checkbox
	auto *const widget = new QWidget;
	auto *layout = new QHBoxLayout(widget);
	layout->addWidget(checkBox);
	widget->setLayout(layout);

	tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
	tableWidget->setItem(0, 1, new QTableWidgetItem("19"));
	tableWidget->setItem(0, 2, new QTableWidgetItem("2"));
	tableWidget->setItem(0, 3, new QTableWidgetItem("36"));
	tableWidget->setCellWidget(0, 4, widget);
	tableWidget->setItem(0, 5, new QTableWidgetItem("Haste"));

	SECTION("Resynchronizing characters test") {
		auto const charHandler = std::make_shared<CharacterHandler>();
		Utils::resynchronizeCharacters(tableWidget, charHandler);

		SECTION("Check stats") {
			REQUIRE(charHandler->getCharacters().at(0)->name == "Fighter");
			REQUIRE(charHandler->getCharacters().at(0)->initiative == 19);
			REQUIRE(charHandler->getCharacters().at(0)->modifier == 2);
			REQUIRE(charHandler->getCharacters().at(0)->hp == 36);
			REQUIRE(charHandler->getCharacters().at(0)->isEnemy == false);
			REQUIRE(charHandler->getCharacters().at(0)->additionalInf == "Haste");
		}
		SECTION("Check filled enemy field") {
			checkBox->setChecked(true);
			Utils::resynchronizeCharacters(tableWidget, charHandler);
			REQUIRE(charHandler->getCharacters().at(0)->isEnemy == true);
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
