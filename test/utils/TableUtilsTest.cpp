#include <QCheckBox>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QWidget>

#include <catch2/catch.hpp>

#include "UtilsTable.hpp"


TEST_CASE("Table Util Testing", "[TableUtils]"){
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

    auto const charHandler = std::make_shared<CharacterHandler>();

    SECTION("Resynchronizing characters test") {
        Utils::Table::resynchronizeCharacters(tableWidget, charHandler);

        SECTION("Check stats") {
            REQUIRE(charHandler->getCharacters().at(0).name == "Fighter");
            REQUIRE(charHandler->getCharacters().at(0).initiative == 19);
            REQUIRE(charHandler->getCharacters().at(0).modifier == 2);
            REQUIRE(charHandler->getCharacters().at(0).hp == 36);
            REQUIRE(charHandler->getCharacters().at(0).isEnemy == false);
            REQUIRE(charHandler->getCharacters().at(0).additionalInf == "Haste");
        }
        SECTION("Check for changed data") {
            tableWidget->setItem(0, 3, new QTableWidgetItem("24"));
            checkBox->setChecked(true);
            Utils::Table::resynchronizeCharacters(tableWidget, charHandler);

            REQUIRE(charHandler->getCharacters().at(0).hp == 24);
            REQUIRE(charHandler->getCharacters().at(0).isEnemy == true);
        }
    }

    SECTION("Table data from widget test") {
        auto tableData = Utils::Table::tableDataFromWidget(tableWidget);

        SECTION("Check stats") {
            REQUIRE(tableData.at(0).at(0) == "Fighter");
            REQUIRE(tableData.at(0).at(1) == "19");
            REQUIRE(tableData.at(0).at(2) == "2");
            REQUIRE(tableData.at(0).at(3) == "36");
            REQUIRE(tableData.at(0).at(4) == false);
            REQUIRE(tableData.at(0).at(5) == "Haste");
        }
        SECTION("Check for changed data") {
            tableWidget->setItem(0, 3, new QTableWidgetItem("24"));
            checkBox->setChecked(true);
            tableData = Utils::Table::tableDataFromWidget(tableWidget);

            REQUIRE(tableData.at(0).at(3) == "24");
            REQUIRE(tableData.at(0).at(4) == true);
        }
    }

    SECTION("Table data from character vector") {
        Utils::Table::resynchronizeCharacters(tableWidget, charHandler);
        auto characters = charHandler->getCharacters();
        auto tableData = Utils::Table::tableDataFromCharacterVector(characters);

        SECTION("Check stats") {
            REQUIRE(tableData.at(0).at(0) == "Fighter");
            REQUIRE(tableData.at(0).at(1) == "19");
            REQUIRE(tableData.at(0).at(2) == "2");
            REQUIRE(tableData.at(0).at(3) == "36");
            REQUIRE(tableData.at(0).at(4) == false);
            REQUIRE(tableData.at(0).at(5) == "Haste");
        }
        SECTION("Check for changed data") {
            tableWidget->setItem(0, 3, new QTableWidgetItem("24"));
            checkBox->setChecked(true);
            Utils::Table::resynchronizeCharacters(tableWidget, charHandler);
            characters = charHandler->getCharacters();
            tableData = Utils::Table::tableDataFromCharacterVector(characters);

            REQUIRE(tableData.at(0).at(3) == "24");
            REQUIRE(tableData.at(0).at(4) == true);
        }
    }
}
