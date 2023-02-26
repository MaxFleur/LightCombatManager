#include <QTableWidget>

#include <catch2/catch.hpp>

#include "UtilsGeneral.hpp"


TEST_CASE("General Util Testing", "[GeneralUtils]") {
    auto *const tableWidget = new QTableWidget(1, 6);

    tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
    tableWidget->setItem(0, 1, new QTableWidgetItem("19"));
    tableWidget->setItem(0, 2, new QTableWidgetItem("2"));
    tableWidget->setItem(0, 3, new QTableWidgetItem("36"));
    tableWidget->setItem(0, 4, new QTableWidgetItem("Testing string"));
    tableWidget->setItem(0, 5, new QTableWidgetItem("Haste"));

    SECTION("Semicolon test") {
        SECTION("Contains semicolon in first column") {
            tableWidget->setItem(0, 0, new QTableWidgetItem("Figh;ter"));
            REQUIRE(Utils::General::containsSemicolon(tableWidget));
        }
        SECTION("Contains semicolon in fifth column") {
            tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
            tableWidget->setItem(0, 5, new QTableWidgetItem("Has;te"));
            REQUIRE(Utils::General::containsSemicolon(tableWidget));
        }
        SECTION("Contains no semicolon") {
            tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
            tableWidget->setItem(0, 5, new QTableWidgetItem("Haste"));
            REQUIRE(!Utils::General::containsSemicolon(tableWidget));
        }
    }

    SECTION("CSV file path test") {
        SECTION("Example Latin") {
            REQUIRE(Utils::General::getCSVName("a/path/to/an/exampleTable.csv") == "exampleTable.csv");
        }
        SECTION("Example Umlaut") {
            REQUIRE(Utils::General::getCSVName("/rändöm/päth/tö/exämpleTäble.csv") == "exämpleTäble.csv");
        }
        SECTION("Example Cyrillic") {
            REQUIRE(Utils::General::getCSVName("путь/к/примеру.csv") == "примеру.csv");
        }
    }
}
