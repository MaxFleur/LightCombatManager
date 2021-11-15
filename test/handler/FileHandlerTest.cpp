#include <QString>
#include <QTableWidget>

#include <catch2/catch.hpp>

#include "FileHandler.hpp"


TEST_CASE("FileHandler Testing", "[FileHandler]"){
	std::shared_ptr<FileHandler> fileHandler = std::make_shared<FileHandler>();

	SECTION("Check format test") {
		SECTION("Functioning table") {
			REQUIRE(fileHandler->getCSVData("./TestingTables/workingTable.csv") == 0);
		}
		SECTION("Broken table") {
			REQUIRE(fileHandler->getCSVData("./TestingTables/brokenTable.csv") == 1);
		}
		SECTION("Non readable/existing table") {
			REQUIRE(fileHandler->getCSVData("./TestingTables/nonexistingTable.csv") == 2);
		}
	}

	SECTION("Check file saving") {
		auto *const tableWidget = new QTableWidget(2, 6);
		tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
		tableWidget->setItem(0, 1, new QTableWidgetItem("19"));
		tableWidget->setItem(0, 2, new QTableWidgetItem("2"));
		tableWidget->setItem(0, 3, new QTableWidgetItem("36"));
		tableWidget->setItem(0, 4, new QTableWidgetItem(""));
		tableWidget->setItem(0, 5, new QTableWidgetItem("Haste"));

		tableWidget->setItem(1, 0, new QTableWidgetItem("Boss"));
		tableWidget->setItem(1, 1, new QTableWidgetItem("21"));
		tableWidget->setItem(1, 2, new QTableWidgetItem("7"));
		tableWidget->setItem(1, 3, new QTableWidgetItem("42"));
		tableWidget->setItem(1, 4, new QTableWidgetItem("X"));
		tableWidget->setItem(1, 5, new QTableWidgetItem(""));

		fileHandler->saveTable(tableWidget, "./test.csv", 0, 1);
		REQUIRE(fileHandler->getCSVData("./test.csv") == 0);
		std::remove("./test.csv");
	}
}
