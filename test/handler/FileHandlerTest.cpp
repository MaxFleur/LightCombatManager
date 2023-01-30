#include <filesystem>
#include <string>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QString>
#include <QTableWidget>
#include <QWidget>

#include <catch2/catch.hpp>

#include "RuleSettings.hpp"
#include "FileHandler.hpp"

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


TEST_CASE_METHOD(FileHandlerTestUtils, "FileHandler Testing", "[FileHandler]"){
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

        auto *const widgetFighter = new QWidget;
        // Centering fighter checkbox
        auto *layoutFighter = new QHBoxLayout(widgetFighter);
        layoutFighter->addWidget(checkBoxFighter);
        widgetFighter->setLayout(layoutFighter);

        tableWidget->setItem(0, 0, new QTableWidgetItem("Fighter"));
        tableWidget->setItem(0, 1, new QTableWidgetItem("19"));
        tableWidget->setItem(0, 2, new QTableWidgetItem("2"));
        tableWidget->setItem(0, 3, new QTableWidgetItem("36"));
        tableWidget->setCellWidget(0, 4, widgetFighter);
        tableWidget->setItem(0, 5, new QTableWidgetItem("Haste"));
        tableWidget->setItem(0, 6, new QTableWidgetItem("0"));

        // Centering boss checkbox
        auto *const widgetBoss = new QWidget;
        auto *layoutBoss = new QHBoxLayout(widgetBoss);
        layoutBoss->addWidget(checkBoxBoss);
        widgetBoss->setLayout(layoutBoss);

        tableWidget->setItem(1, 0, new QTableWidgetItem("Boss"));
        tableWidget->setItem(1, 1, new QTableWidgetItem("21"));
        tableWidget->setItem(1, 2, new QTableWidgetItem("7"));
        tableWidget->setItem(1, 3, new QTableWidgetItem("42"));
        tableWidget->setCellWidget(1, 4, widgetBoss);
        tableWidget->setItem(1, 5, new QTableWidgetItem(""));
        tableWidget->setItem(1, 6, new QTableWidgetItem("1"));

        auto const ruleSettings = std::make_shared<RuleSettings>();
        ruleSettings->ruleset = RuleSettings::Ruleset::PATHFINDER_2E;
        ruleSettings->rollAutomatical = true;

        const auto tableSaved = fileHandler->saveTable(tableWidget, "./test.csv", 0, 1,
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
            REQUIRE(rowData[5] == "Haste");
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
