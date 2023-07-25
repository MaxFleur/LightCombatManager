#include <QApplication>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QWidget>

#include <catch2/catch.hpp>

#include "AdditionalInfoData.hpp"
#include "AdditionalInfoWidget.hpp"
#include "UtilsTable.hpp"


TEST_CASE("Table Util Testing", "[TableUtils]") {
    auto *const tableWidget = new QTableWidget(1, 6);
    auto *const checkBox = new QCheckBox;
    checkBox->setChecked(false);
    auto* const additionalInformationWidget = new AdditionalInfoWidget;

    AdditionalInfoData::StatusEffect effect{ "Shaken", false, 2 };
    QVector<AdditionalInfoData::StatusEffect> statusEffects{ effect };
    additionalInformationWidget->setMainInfoText("Haste");
    additionalInformationWidget->setStatusEffects(statusEffects);

    const auto createCellWidget = [&] (QWidget* widget) {
        auto *const cellWidget = new QWidget;
        auto *const cellLayout = new QHBoxLayout(cellWidget);
        cellLayout->addWidget(widget);
        cellWidget->setLayout(cellLayout);

        return cellWidget;
    };

    tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
    tableWidget->setItem(0, 1, new QTableWidgetItem("19"));
    tableWidget->setItem(0, 2, new QTableWidgetItem("2"));
    tableWidget->setItem(0, 3, new QTableWidgetItem("36"));
    tableWidget->setCellWidget(0, 4, createCellWidget(checkBox));
    tableWidget->setCellWidget(0, 5, createCellWidget(additionalInformationWidget));

    auto *const checkBoxSecondRow = new QCheckBox;
    checkBoxSecondRow->setChecked(true);
    auto* const additionalInformationWidgetSecondRow = new AdditionalInfoWidget;

    tableWidget->setRowCount(2);
    tableWidget->setItem(1, 0, new QTableWidgetItem("Enemy"));
    tableWidget->setItem(1, 1, new QTableWidgetItem("23"));
    tableWidget->setItem(1, 2, new QTableWidgetItem("5"));
    tableWidget->setItem(1, 3, new QTableWidgetItem("66"));
    tableWidget->setCellWidget(1, 4, createCellWidget(checkBoxSecondRow));
    tableWidget->setCellWidget(1, 5, createCellWidget(additionalInformationWidgetSecondRow));

    auto const charHandler = std::make_shared<CharacterHandler>();

    SECTION("Resynchronizing characters test") {
        Utils::Table::resynchronizeCharacters(tableWidget, charHandler);

        SECTION("Check stats") {
            REQUIRE(charHandler->getCharacters().at(0).name == "Fighter");
            REQUIRE(charHandler->getCharacters().at(0).initiative == 19);
            REQUIRE(charHandler->getCharacters().at(0).modifier == 2);
            REQUIRE(charHandler->getCharacters().at(0).hp == 36);
            REQUIRE(charHandler->getCharacters().at(0).isEnemy == false);
            REQUIRE(charHandler->getCharacters().at(0).additionalInformation.mainInfo == "Haste");
            REQUIRE(charHandler->getCharacters().at(0).additionalInformation.statusEffects.at(0).name == "Shaken");
            REQUIRE(charHandler->getCharacters().at(0).additionalInformation.statusEffects.at(0).isPermanent == false);
            REQUIRE(charHandler->getCharacters().at(0).additionalInformation.statusEffects.at(0).duration == 2);
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
            const auto converted = tableData.at(0).at(5).value<AdditionalInfoData::AdditionalInformation>();

            REQUIRE(tableData.at(0).at(0) == "Fighter");
            REQUIRE(tableData.at(0).at(1) == "19");
            REQUIRE(tableData.at(0).at(2) == "2");
            REQUIRE(tableData.at(0).at(3) == "36");
            REQUIRE(tableData.at(0).at(4) == false);
            REQUIRE(converted.mainInfo == "Haste");
            REQUIRE(converted.statusEffects.at(0).name == "Shaken");
            REQUIRE(converted.statusEffects.at(0).isPermanent == false);
            REQUIRE(converted.statusEffects.at(0).duration == 2);
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
        auto tableData = Utils::Table::tableDataFromCharacterVector(charHandler);

        SECTION("Check stats") {
            const auto converted = tableData.at(0).at(5).value<AdditionalInfoData::AdditionalInformation>();

            REQUIRE(tableData.at(0).at(0) == "Fighter");
            REQUIRE(tableData.at(0).at(1) == "19");
            REQUIRE(tableData.at(0).at(2) == "2");
            REQUIRE(tableData.at(0).at(3) == "36");
            REQUIRE(tableData.at(0).at(4) == false);
            REQUIRE(converted.mainInfo == "Haste");
            REQUIRE(converted.statusEffects.at(0).name == "Shaken");
            REQUIRE(converted.statusEffects.at(0).isPermanent == false);
            REQUIRE(converted.statusEffects.at(0).duration == 2);
        }
        SECTION("Check for changed data") {
            tableWidget->setItem(0, 3, new QTableWidgetItem("24"));
            checkBox->setChecked(true);
            Utils::Table::resynchronizeCharacters(tableWidget, charHandler);
            characters = charHandler->getCharacters();
            tableData = Utils::Table::tableDataFromCharacterVector(charHandler);

            REQUIRE(tableData.at(0).at(3) == "24");
            REQUIRE(tableData.at(0).at(4) == true);
        }
    }

    SECTION("Tooltip test") {
        SECTION("Set tooltip") {
            Utils::Table::setIniColumnTooltips(tableWidget, false);

            REQUIRE(tableWidget->item(0, 1)->toolTip() == "Calculation: Rolled Value 17, Modifier 2");
            REQUIRE(tableWidget->item(1, 1)->toolTip() == "Calculation: Rolled Value 18, Modifier 5");
        }
        SECTION("Reset tooltip") {
            Utils::Table::setIniColumnTooltips(tableWidget, true);

            REQUIRE(tableWidget->item(0, 1)->toolTip() == "");
            REQUIRE(tableWidget->item(1, 1)->toolTip() == "");
        }
    }

    SECTION("Row coloring test") {
        const auto color = QApplication::palette().color(QPalette::Base);

        SECTION("Set color") {
            Utils::Table::setTableRowColor(tableWidget, false);

            REQUIRE(tableWidget->item(0, 0)->background().color() == QColor(12, 123, 220, 60));
            REQUIRE(tableWidget->cellWidget(0, 4)->palette().color(QPalette::Base) == QColor(12, 123, 220, 60));
            REQUIRE(tableWidget->cellWidget(0, 5)->palette().color(QPalette::Base) == QColor(12, 123, 220, 60));
            REQUIRE(tableWidget->cellWidget(0, 5)->palette().color(QPalette::Button) == QColor(12, 123, 220, 10));
            REQUIRE(tableWidget->item(1, 0)->background().color() == QColor(255, 194, 10, 60));
            REQUIRE(tableWidget->cellWidget(1, 4)->palette().color(QPalette::Base) == QColor(255, 194, 10, 60));
            REQUIRE(tableWidget->cellWidget(1, 5)->palette().color(QPalette::Button) == QColor(255, 194, 10, 10));
        }
        SECTION("Reset color") {
            Utils::Table::setTableRowColor(tableWidget, true);

            REQUIRE(tableWidget->item(0, 0)->background().color() == color);
            REQUIRE(tableWidget->cellWidget(0, 4)->palette().color(QPalette::Base) == color);
            REQUIRE(tableWidget->cellWidget(0, 5)->palette().color(QPalette::Base) == color);
            REQUIRE(tableWidget->cellWidget(0, 5)->palette().color(QPalette::Button) == color);
            REQUIRE(tableWidget->item(1, 0)->background().color() == color);
            REQUIRE(tableWidget->cellWidget(1, 4)->palette().color(QPalette::Base) == color);
            REQUIRE(tableWidget->cellWidget(1, 5)->palette().color(QPalette::Base) == color);
            REQUIRE(tableWidget->cellWidget(1, 5)->palette().color(QPalette::Button) == color);
        }
    }

    SECTION("Set row and player test") {
        auto* const roundCounterLabel = new QLabel;
        auto* const currentPlayerLabel = new QLabel;

        SECTION("Set first row") {
            Utils::Table::setRowAndPlayer(tableWidget, roundCounterLabel, currentPlayerLabel, 0);

            REQUIRE(tableWidget->item(0, 0)->font().bold());
            REQUIRE(!tableWidget->item(1, 0)->font().bold());
            REQUIRE(currentPlayerLabel->text() == "Current: Fighter");
        }
        SECTION("Set second row") {
            Utils::Table::setRowAndPlayer(tableWidget, roundCounterLabel, currentPlayerLabel, 1);

            REQUIRE(!tableWidget->item(0, 0)->font().bold());
            REQUIRE(tableWidget->item(1, 0)->font().bold());
            REQUIRE(currentPlayerLabel->text() == "Current: Enemy");
        }
        SECTION("Set second row") {
            tableWidget->setRowCount(0);
            Utils::Table::setRowAndPlayer(tableWidget, roundCounterLabel, currentPlayerLabel, 0);

            REQUIRE(currentPlayerLabel->text() == "Current: None");
        }
    }
}
