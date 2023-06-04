#include <filesystem>
#include <string>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QWidget>

#include <catch2/catch.hpp>

#include "AdditionalInfoData.hpp"
#include "AdditionalInfoWidget.hpp"
#include "FileHandler.hpp"
#include "RuleSettings.hpp"
#include "UtilsGeneral.hpp"
#include "UtilsTable.hpp"

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
        auto *const tableWidget = new QTableWidget(2, 6);

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

        tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
        tableWidget->setItem(0, 1, new QTableWidgetItem("19"));
        tableWidget->setItem(0, 2, new QTableWidgetItem("2"));
        tableWidget->setItem(0, 3, new QTableWidgetItem("36"));
        tableWidget->setCellWidget(0, 4, createCellWidget(checkBoxFighter));
        tableWidget->setCellWidget(0, 5, createCellWidget(additionalInfoWidgetFighter));
        tableWidget->setItem(0, 6, new QTableWidgetItem("0"));

        tableWidget->setItem(1, 0, new QTableWidgetItem("Boss"));
        tableWidget->setItem(1, 1, new QTableWidgetItem("21"));
        tableWidget->setItem(1, 2, new QTableWidgetItem("7"));
        tableWidget->setItem(1, 3, new QTableWidgetItem("42"));
        tableWidget->setCellWidget(1, 4, createCellWidget(checkBoxBoss));
        tableWidget->setCellWidget(1, 5, createCellWidget(additionalInfoWidgetBoss));
        tableWidget->setItem(1, 6, new QTableWidgetItem("1"));

        auto const ruleSettings = std::make_shared<RuleSettings>();
        ruleSettings->ruleset = RuleSettings::Ruleset::PATHFINDER_2E;
        ruleSettings->rollAutomatical = true;

        const auto tableDataWidget = Utils::Table::tableDataFromWidget(tableWidget);
        const auto tableSaved = fileHandler->saveTable(tableDataWidget, "./test.csv", 0, 1,
                                                       ruleSettings->ruleset, ruleSettings->rollAutomatical);
        SECTION("File format correct") {
            REQUIRE(fileHandler->getCSVData("./test.csv") == 0);
        }

        SECTION("File content correct") {
            const auto code = fileHandler->getCSVData("./test.csv");

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
