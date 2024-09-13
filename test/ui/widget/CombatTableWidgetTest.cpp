#include "AdditionalInfoData.hpp"
#include "AdditionalInfoWidget.hpp"
#include "CharacterHandler.hpp"
#include "CombatTableWidget.hpp"

#ifdef CATCH2_V3
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QWidget>

TEST_CASE("Combat Table Testing", "[TableUtils]") {
    auto characterHandler = std::make_shared<CharacterHandler>();
    auto *const combatTableWidget = new CombatTableWidget(characterHandler, 720);
    combatTableWidget->setRowCount(2);
    combatTableWidget->setColumnCount(6);

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

    combatTableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
    combatTableWidget->setItem(0, 1, new QTableWidgetItem("19"));
    combatTableWidget->setItem(0, 2, new QTableWidgetItem("2"));
    combatTableWidget->setItem(0, 3, new QTableWidgetItem("36"));
    combatTableWidget->setItem(0, 4, new QTableWidgetItem);
    combatTableWidget->item(0, 4)->setCheckState(Qt::Unchecked);
    combatTableWidget->setCellWidget(0, 5, createCellWidget(additionalInformationWidget));

    auto* const additionalInformationWidgetSecondRow = new AdditionalInfoWidget;

    combatTableWidget->setRowCount(2);
    combatTableWidget->setItem(1, 0, new QTableWidgetItem("Enemy"));
    combatTableWidget->setItem(1, 1, new QTableWidgetItem("23"));
    combatTableWidget->setItem(1, 2, new QTableWidgetItem("5"));
    combatTableWidget->setItem(1, 3, new QTableWidgetItem("66"));
    combatTableWidget->setItem(1, 4, new QTableWidgetItem);
    combatTableWidget->item(1, 4)->setCheckState(Qt::Checked);
    combatTableWidget->setCellWidget(1, 5, createCellWidget(additionalInformationWidgetSecondRow));

    SECTION("Resynchronizing characters test") {
        combatTableWidget->resynchronizeCharacters();

        SECTION("Check stats") {
            REQUIRE(characterHandler->getCharacters().at(0).name == "Fighter");
            REQUIRE(characterHandler->getCharacters().at(0).initiative == 19);
            REQUIRE(characterHandler->getCharacters().at(0).modifier == 2);
            REQUIRE(characterHandler->getCharacters().at(0).hp == 36);
            REQUIRE(characterHandler->getCharacters().at(0).isEnemy == false);
            REQUIRE(characterHandler->getCharacters().at(0).additionalInfoData.mainInfoText == "Haste");
            REQUIRE(characterHandler->getCharacters().at(0).additionalInfoData.statusEffects.at(0).name == "Shaken");
            REQUIRE(characterHandler->getCharacters().at(0).additionalInfoData.statusEffects.at(0).isPermanent == false);
            REQUIRE(characterHandler->getCharacters().at(0).additionalInfoData.statusEffects.at(0).duration == 2);
        }
        SECTION("Check for changed data") {
            combatTableWidget->setItem(0, 3, new QTableWidgetItem("24"));
            combatTableWidget->item(0, 4)->setCheckState(Qt::Checked);
            combatTableWidget->resynchronizeCharacters();

            REQUIRE(characterHandler->getCharacters().at(0).hp == 24);
            REQUIRE(characterHandler->getCharacters().at(0).isEnemy == true);
        }
    }

    SECTION("Set row and player test") {
        auto* const roundCounterLabel = new QLabel;
        auto* const currentPlayerLabel = new QLabel;

        SECTION("Set first row") {
            combatTableWidget->setRowAndPlayer(roundCounterLabel, currentPlayerLabel, 0);

            REQUIRE(combatTableWidget->item(0, 0)->font().bold());
            REQUIRE(!combatTableWidget->item(1, 0)->font().bold());
            REQUIRE(currentPlayerLabel->text() == "Current: Fighter");
        }
        SECTION("Set second row") {
            combatTableWidget->setRowAndPlayer(roundCounterLabel, currentPlayerLabel, 1);

            REQUIRE(!combatTableWidget->item(0, 0)->font().bold());
            REQUIRE(combatTableWidget->item(1, 0)->font().bold());
            REQUIRE(currentPlayerLabel->text() == "Current: Enemy");
        }
        SECTION("Set second row") {
            combatTableWidget->setRowCount(0);
            combatTableWidget->setRowAndPlayer(roundCounterLabel, currentPlayerLabel, 0);

            REQUIRE(currentPlayerLabel->text() == "Current: None");
        }
    }

    SECTION("Row coloring test") {
        const auto color = QApplication::palette().color(QPalette::Base);

        SECTION("Set color") {
            combatTableWidget->setTableRowColor(false);

            REQUIRE(combatTableWidget->item(0, 0)->background().color() == QColor(12, 123, 220, 60));
            REQUIRE(combatTableWidget->cellWidget(0, 5)->palette().color(QPalette::Base) == QColor(12, 123, 220, 60));
            REQUIRE(combatTableWidget->item(1, 0)->background().color() == QColor(255, 194, 10, 60));
        }
        SECTION("Reset color") {
            combatTableWidget->setTableRowColor(true);

            REQUIRE(combatTableWidget->item(0, 0)->background().color() == color);
            REQUIRE(combatTableWidget->cellWidget(0, 5)->palette().color(QPalette::Base) == color);
            REQUIRE(combatTableWidget->item(1, 0)->background().color() == color);
            REQUIRE(combatTableWidget->cellWidget(1, 5)->palette().color(QPalette::Base) == color);
        }
    }

    SECTION("Tooltip test") {
        SECTION("Set tooltip") {
            combatTableWidget->setIniColumnTooltips(false);

            REQUIRE(combatTableWidget->item(0, 1)->toolTip() == "Calculation: Rolled Value 17, Modifier 2");
            REQUIRE(combatTableWidget->item(1, 1)->toolTip() == "Calculation: Rolled Value 18, Modifier 5");
        }
        SECTION("Reset tooltip") {
            combatTableWidget->setIniColumnTooltips(true);

            REQUIRE(combatTableWidget->item(0, 1)->toolTip() == "");
            REQUIRE(combatTableWidget->item(1, 1)->toolTip() == "");
        }
    }

    SECTION("Table data from widget test") {
        auto tableData = combatTableWidget->tableDataFromWidget();

        SECTION("Check stats") {
            const auto converted = tableData.at(0).at(5).value<AdditionalInfoData>();

            REQUIRE(tableData.at(0).at(0) == "Fighter");
            REQUIRE(tableData.at(0).at(1) == "19");
            REQUIRE(tableData.at(0).at(2) == "2");
            REQUIRE(tableData.at(0).at(3) == "36");
            REQUIRE(tableData.at(0).at(4) == false);
            REQUIRE(converted.mainInfoText == "Haste");
            REQUIRE(converted.statusEffects.at(0).name == "Shaken");
            REQUIRE(converted.statusEffects.at(0).isPermanent == false);
            REQUIRE(converted.statusEffects.at(0).duration == 2);
        }
        SECTION("Check for changed data") {
            combatTableWidget->setItem(0, 3, new QTableWidgetItem("24"));
            combatTableWidget->item(0, 4)->setCheckState(Qt::Checked);
            tableData = combatTableWidget->tableDataFromWidget();

            REQUIRE(tableData.at(0).at(3) == "24");
            REQUIRE(tableData.at(0).at(4) == true);
        }
    }

    SECTION("Table data from character vector") {
        combatTableWidget->resynchronizeCharacters();
        auto characters = characterHandler->getCharacters();
        auto tableData = combatTableWidget->tableDataFromCharacterVector();

        SECTION("Check stats") {
            const auto converted = tableData.at(0).at(5).value<AdditionalInfoData>();

            REQUIRE(tableData.at(0).at(0) == "Fighter");
            REQUIRE(tableData.at(0).at(1) == "19");
            REQUIRE(tableData.at(0).at(2) == "2");
            REQUIRE(tableData.at(0).at(3) == "36");
            REQUIRE(tableData.at(0).at(4) == false);
            REQUIRE(converted.mainInfoText == "Haste");
            REQUIRE(converted.statusEffects.at(0).name == "Shaken");
            REQUIRE(converted.statusEffects.at(0).isPermanent == false);
            REQUIRE(converted.statusEffects.at(0).duration == 2);
        }
        SECTION("Check for changed data") {
            combatTableWidget->setItem(0, 3, new QTableWidgetItem("24"));
            combatTableWidget->item(0, 4)->setCheckState(Qt::Checked);
            combatTableWidget->resynchronizeCharacters();
            characters = characterHandler->getCharacters();
            tableData = combatTableWidget->tableDataFromCharacterVector();

            REQUIRE(tableData.at(0).at(3) == "24");
            REQUIRE(tableData.at(0).at(4) == true);
        }
    }

    SECTION("Set row and player test") {
        const unsigned int rowHeightFirst = combatTableWidget->rowHeight(0);
        const unsigned int rowHeightSecond = combatTableWidget->rowHeight(1);
        REQUIRE(combatTableWidget->getHeight() == rowHeightFirst + rowHeightSecond + 140);
    }
}
