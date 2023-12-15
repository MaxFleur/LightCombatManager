#include "AdditionalInfoData.hpp"
#include "AdditionalInfoWidget.hpp"
#include "CharacterHandler.hpp"
#include "CombatTableWidget.hpp"
#include "FileHandler.hpp"
#include "RuleSettings.hpp"
#include "UtilsGeneral.hpp"

#include <catch2/catch.hpp>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QWidget>

#include <filesystem>

class FileHandlerTestUtils
{
public:
    QString
    resolvePath(std::filesystem::path relativeDir)
    {
        auto currentDir = std::filesystem::current_path();

        while (currentDir.has_parent_path()) {
            const auto combinedPath = currentDir / relativeDir;

            if (std::filesystem::exists(combinedPath)) {
                return QString::fromStdString(combinedPath.u8string());
            }
            currentDir = currentDir.parent_path();
        }
        throw std::runtime_error("File not found!");
    }
};


TEST_CASE_METHOD(FileHandlerTestUtils, "FileHandler Testing", "[FileHandler]") {
    auto const fileHandler = std::make_shared<FileHandler>();

    SECTION("Check format test") {
        SECTION("Functioning table") {
            REQUIRE(fileHandler->getCSVStatus(resolvePath("resources/tables/workingTable.csv")) == 0);
        }
        SECTION("Broken table") {
            REQUIRE(fileHandler->getCSVStatus(resolvePath("resources/tables/brokenTable.csv")) == 1);
        }
        SECTION("Non readable/existing table") {
            REQUIRE(fileHandler->getCSVStatus("dir/to/nonexisting/table.csv") == 2);
        }
    }

    SECTION("Check file saving") {
        auto characterHandler = std::make_shared<CharacterHandler>();
        auto *const combatTableWidget = new CombatTableWidget(characterHandler);
        combatTableWidget->setRowCount(2);
        combatTableWidget->setColumnCount(6);

        auto *const checkBoxFighter = new QCheckBox;
        checkBoxFighter->setChecked(false);
        auto *const checkBoxBoss = new QCheckBox;
        checkBoxBoss->setChecked(true);

        auto *const additionalInfoWidgetFighter = new AdditionalInfoWidget;

        AdditionalInfoData::StatusEffect effect1{ "Shaken", false, 2 };
        AdditionalInfoData::StatusEffect effect2{ "Exhausted", true, 0 };
        QVector<AdditionalInfoData::StatusEffect> statusEffects { effect1, effect2 };

        additionalInfoWidgetFighter->setMainInfoText("Haste");
        additionalInfoWidgetFighter->setStatusEffects(statusEffects);

        auto *const additionalInfoWidgetBoss = new AdditionalInfoWidget;

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
        combatTableWidget->setCellWidget(0, 4, createCellWidget(checkBoxFighter));
        combatTableWidget->setCellWidget(0, 5, createCellWidget(additionalInfoWidgetFighter));
        combatTableWidget->setItem(0, 6, new QTableWidgetItem("0"));

        combatTableWidget->setItem(1, 0, new QTableWidgetItem("Boss"));
        combatTableWidget->setItem(1, 1, new QTableWidgetItem("21"));
        combatTableWidget->setItem(1, 2, new QTableWidgetItem("7"));
        combatTableWidget->setItem(1, 3, new QTableWidgetItem("42"));
        combatTableWidget->setCellWidget(1, 4, createCellWidget(checkBoxBoss));
        combatTableWidget->setCellWidget(1, 5, createCellWidget(additionalInfoWidgetBoss));
        combatTableWidget->setItem(1, 6, new QTableWidgetItem("1"));

        RuleSettings ruleSettings;
        ruleSettings.ruleset = RuleSettings::Ruleset::PATHFINDER_2E;
        ruleSettings.rollAutomatical = true;

        const auto tableData = combatTableWidget->tableDataFromWidget();
        const auto tableSaved = fileHandler->saveTable(tableData, "./test.csv", 0, 1,
                                                       ruleSettings.ruleset, ruleSettings.rollAutomatical);

        SECTION("Table successfully saved") {
            REQUIRE(tableSaved == true);
        }

        SECTION("File format and content correct") {
            const auto codeCSVStatus = fileHandler->getCSVStatus("./test.csv");
            REQUIRE(codeCSVStatus == 0);

            auto rowOfData = fileHandler->getData().split("\n");
            QStringList rowData;

            rowData = rowOfData.at(1).split(";");
            REQUIRE(rowData[0] == "Fighter");
            REQUIRE(rowData[1] == "19");
            REQUIRE(rowData[2] == "2");
            REQUIRE(rowData[3] == "36");
            REQUIRE(rowData[4] == "false");
            REQUIRE(rowData[5] == "Haste---Shaken+0+2--Exhausted+1+0--");
            REQUIRE(rowData[6] == "0");
            REQUIRE(rowData[7] == "1");
            REQUIRE(rowData[8] == "1");
            REQUIRE(rowData[9] == "1");

            rowData = rowOfData.at(2).split(";");

            REQUIRE(rowData[0] == "Boss");
            REQUIRE(rowData[1] == "21");
            REQUIRE(rowData[2] == "7");
            REQUIRE(rowData[3] == "42");
            REQUIRE(rowData[4] == "true");
            REQUIRE(rowData[5] == "");
        }

        std::remove("./test.csv");
    }
}
