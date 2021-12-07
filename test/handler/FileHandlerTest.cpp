#include <filesystem>
#include <string>

#include <QString>
#include <QTableWidget>

#include <catch2/catch.hpp>

#include "FileHandler.hpp"

class FileHandlerTestUtils
{
public:

	QString
	resolvePath(std::filesystem::path relativeDir)
	{
		auto currentDir = std::filesystem::current_path();

		while (currentDir.has_parent_path()) {
			auto combinedPath = currentDir / relativeDir;

			if (std::filesystem::exists(combinedPath)) {
				return QString::fromStdString(combinedPath.u8string());
			}
			currentDir = currentDir.parent_path();
		}
		throw std::runtime_error("File not found!");
	}
};


TEST_CASE_METHOD(FileHandlerTestUtils, "FileHandler Testing", "[FileHandler]"){
	std::shared_ptr<FileHandler> fileHandler = std::make_shared<FileHandler>();

	SECTION("Check format test") {
		SECTION("Functioning table") {
			REQUIRE(fileHandler->getCSVData(resolvePath("resources/tables/workingTable.csv")) == 0);
		}
		SECTION("Broken table") {
			REQUIRE(fileHandler->getCSVData(resolvePath("resources/tables/brokenTable.csv")) == 1);
		}
		SECTION("Non readable/existing table") {
			REQUIRE(fileHandler->getCSVData("dir/to/nonexisting/table.csv") == 2);
		}
	}

	SECTION("Check file saving") {
		auto *const tableWidget = new QTableWidget(2, 7);
		tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
		tableWidget->setItem(0, 1, new QTableWidgetItem("19"));
		tableWidget->setItem(0, 2, new QTableWidgetItem("2"));
		tableWidget->setItem(0, 3, new QTableWidgetItem("36"));
		tableWidget->setItem(0, 4, new QTableWidgetItem(""));
		tableWidget->setItem(0, 5, new QTableWidgetItem("Haste"));
		tableWidget->setItem(0, 6, new QTableWidgetItem("0"));

		tableWidget->setItem(1, 0, new QTableWidgetItem("Boss"));
		tableWidget->setItem(1, 1, new QTableWidgetItem("21"));
		tableWidget->setItem(1, 2, new QTableWidgetItem("7"));
		tableWidget->setItem(1, 3, new QTableWidgetItem("42"));
		tableWidget->setItem(1, 4, new QTableWidgetItem("X"));
		tableWidget->setItem(1, 5, new QTableWidgetItem(""));
		tableWidget->setItem(1, 6, new QTableWidgetItem("1"));

		fileHandler->saveTable(tableWidget, "./test.csv", 0, 1);
		REQUIRE(fileHandler->getCSVData("./test.csv") == 0);
		std::remove("./test.csv");
	}
}
