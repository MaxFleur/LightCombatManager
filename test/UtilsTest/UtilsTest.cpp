#include <QString>
#include <QTableWidget>

#include <catch2/catch.hpp>

#include "Utils.hpp"


TEST_CASE("Util Testing", "[Utils]"){
	SECTION("Semicolon test"){
		SECTION("Contains semicolon in first column"){
			auto *const tableWidget = new QTableWidget(1, 6);
			tableWidget->setItem(0, 0, new QTableWidgetItem("Figh;ter"));
			tableWidget->setItem(0, 1, new QTableWidgetItem("19"));
			tableWidget->setItem(0, 2, new QTableWidgetItem("2"));
			tableWidget->setItem(0, 3, new QTableWidgetItem("36"));
			tableWidget->setItem(0, 4, new QTableWidgetItem(""));
			tableWidget->setItem(0, 5, new QTableWidgetItem("Does not contain a semicolon"));
			REQUIRE(Utils::containsSemicolon(tableWidget));
		}

		SECTION("Contains semicolon in sixth column"){
			auto *const tableWidget = new QTableWidget(1, 6);
			tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
			tableWidget->setItem(0, 1, new QTableWidgetItem("19"));
			tableWidget->setItem(0, 2, new QTableWidgetItem("2"));
			tableWidget->setItem(0, 3, new QTableWidgetItem("36"));
			tableWidget->setItem(0, 4, new QTableWidgetItem(""));
			tableWidget->setItem(0, 5, new QTableWidgetItem("Does ; contain a semicolon"));
			REQUIRE(Utils::containsSemicolon(tableWidget));
		}
		SECTION("Contains no semicolon"){
			auto *const tableWidget = new QTableWidget(1, 6);
			tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
			tableWidget->setItem(0, 1, new QTableWidgetItem("19"));
			tableWidget->setItem(0, 2, new QTableWidgetItem("2"));
			tableWidget->setItem(0, 3, new QTableWidgetItem("36"));
			tableWidget->setItem(0, 4, new QTableWidgetItem(""));
			tableWidget->setItem(0, 5, new QTableWidgetItem("Does not contain a semicolon"));
			REQUIRE(!Utils::containsSemicolon(tableWidget));
		}
	}
}
