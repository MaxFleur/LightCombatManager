#include <catch2/catch.hpp>

#include <QHBoxLayout>
#include <QTableWidget>

#include "AdditionalInfoWidget.hpp"
#include "UtilsGeneral.hpp"

TEST_CASE("General Util Testing", "[GeneralUtils]") {
    auto *const tableWidget = new QTableWidget(1, 6);

    auto* const additionalInformationWidget = new AdditionalInfoWidget;
    additionalInformationWidget->setMainInfoText("Haste");
    auto *const cellWidget = new QWidget;
    auto *const cellLayout = new QHBoxLayout(cellWidget);
    cellLayout->addWidget(additionalInformationWidget);
    cellWidget->setLayout(cellLayout);

    tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
    tableWidget->setItem(0, 1, new QTableWidgetItem("19"));
    tableWidget->setItem(0, 2, new QTableWidgetItem("2"));
    tableWidget->setItem(0, 3, new QTableWidgetItem("36"));
    tableWidget->setItem(0, 4, new QTableWidgetItem("Testing string"));
    tableWidget->setCellWidget(0, 5, cellWidget);

    SECTION("Semicolon test") {
        SECTION("Contains semicolon in first column") {
            tableWidget->setItem(0, 0, new QTableWidgetItem("Figh;ter"));
            REQUIRE(Utils::General::containsSemicolon(tableWidget));
        }
        SECTION("Contains semicolon in fifth column") {
            tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
            additionalInformationWidget->setMainInfoText("Ha;ste");
            REQUIRE(Utils::General::containsSemicolon(tableWidget));
        }
        SECTION("Contains no semicolon") {
            tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
            additionalInformationWidget->setMainInfoText("Haste");
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

    SECTION("Converting stored string to additional information test") {
        SECTION("Basic Example") {
            const QString testString = "Haste and other effects---Dazed+1+3--";
            const auto additionalInformation = Utils::General::convertStringToAdditionalInfoData(testString);

            REQUIRE(additionalInformation.mainInfo == "Haste and other effects");
            REQUIRE(additionalInformation.statusEffects.at(0).name == "Dazed");
            REQUIRE(additionalInformation.statusEffects.at(0).isPermanent == 1);
            REQUIRE(additionalInformation.statusEffects.at(0).duration == 3);
        }
        SECTION("Advanced Example") {
            const QString testString = "Fire Resistance, ------Dazed+1+3--Grappled+0+4";
            const auto additionalInformation = Utils::General::convertStringToAdditionalInfoData(testString);

            REQUIRE(additionalInformation.mainInfo == "Fire Resistance, ---");
            REQUIRE(additionalInformation.statusEffects.at(0).name == "Dazed");
            REQUIRE(additionalInformation.statusEffects.at(0).isPermanent == 1);
            REQUIRE(additionalInformation.statusEffects.at(0).duration == 3);
            REQUIRE(additionalInformation.statusEffects.at(1).name == "Grappled");
            REQUIRE(additionalInformation.statusEffects.at(1).isPermanent == 0);
            REQUIRE(additionalInformation.statusEffects.at(1).duration == 4);
        }
    }
}
