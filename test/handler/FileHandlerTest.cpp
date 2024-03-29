#include "AdditionalInfoData.hpp"
#include "AdditionalInfoWidget.hpp"
#include "CharacterHandler.hpp"
#include "CombatTableWidget.hpp"
#include "FileHandler.hpp"
#include "RuleSettings.hpp"

#include <catch2/catch.hpp>

#include <QCheckBox>
#include <QFile>
#include <QHBoxLayout>
#include <QJsonDocument>
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
        const auto tableSaved = fileHandler->writeTableToFile(tableData, "./test.lcm", 0, 1,
                                                              ruleSettings.ruleset, ruleSettings.rollAutomatical);

        SECTION("Table successfully saved") {
            REQUIRE(tableSaved == true);
        }

        SECTION("File format and content correct") {
            const auto codeCSVStatus = fileHandler->getLCMStatus("./test.lcm");
            REQUIRE(codeCSVStatus == 0);

            const auto& jsonObject = fileHandler->getData();
            REQUIRE(jsonObject.value("row_entered").toInt() == 0);
            REQUIRE(jsonObject.value("round_counter").toInt() == 1);
            REQUIRE(jsonObject.value("ruleset").toInt() == 1);
            REQUIRE(jsonObject.value("roll_automatically").toBool() == true);

            const auto& charactersObject = jsonObject.value("characters").toObject();

            const auto& firstCharacterObject = charactersObject.value("0").toObject();
            REQUIRE(firstCharacterObject.value("name").toString() == "Fighter");
            REQUIRE(firstCharacterObject.value("initiative").toInt() == 19);
            REQUIRE(firstCharacterObject.value("modifier").toInt() == 2);
            REQUIRE(firstCharacterObject.value("hp").toInt() == 36);
            REQUIRE(firstCharacterObject.value("is_enemy").toBool() == false);

            auto additionalInfoObject = firstCharacterObject.value("additional_info").toObject();
            REQUIRE(additionalInfoObject.value("main_info").toString() == "Haste");

            auto statusEffectsObject = additionalInfoObject.value("status_effects").toObject();
            const auto& firstEffectObject = statusEffectsObject.value("0").toObject();
            const auto& secondEffectObject = statusEffectsObject.value("1").toObject();
            REQUIRE(firstEffectObject.value("is_permanent").toBool() == false);
            REQUIRE(firstEffectObject.value("duration").toInt() == 2);
            REQUIRE(secondEffectObject.value("is_permanent").toBool() == true);
            REQUIRE(secondEffectObject.value("duration").toInt() == 0);

            const auto& secondCharacterObject = charactersObject.value("1").toObject();
            REQUIRE(secondCharacterObject.value("name").toString() == "Boss");
            REQUIRE(secondCharacterObject.value("initiative").toInt() == 21);
            REQUIRE(secondCharacterObject.value("modifier").toInt() == 7);
            REQUIRE(secondCharacterObject.value("hp").toInt() == 42);
            REQUIRE(secondCharacterObject.value("is_enemy").toBool() == true);

            additionalInfoObject = secondCharacterObject.value("additional_info").toObject();
            REQUIRE(additionalInfoObject.value("main_info").toString() == "");

            statusEffectsObject = additionalInfoObject.value("status_effects").toObject();
            REQUIRE(statusEffectsObject.empty() == true);
        }

        SECTION("Check format test") {
            SECTION("Functioning table") {
                REQUIRE(fileHandler->getLCMStatus(resolvePath("./test.lcm")) == 0);
            }
            SECTION("Broken table") {
                // Incomplete json object
                QJsonObject jsonObject;
                jsonObject["row_entered"] = 2;
                jsonObject["round_counter"] = 3;

                // Write to file
                auto byteArray = QJsonDocument(jsonObject).toJson();
                QFile fileOut("./broken.lcm");
                fileOut.open(QIODevice::WriteOnly);

                fileOut.write(byteArray);
                REQUIRE(fileHandler->getLCMStatus(resolvePath("./broken.lcm")) == 1);
                std::remove("./broken.lcm");
            }
            SECTION("Non readable/existing table") {
                REQUIRE(fileHandler->getLCMStatus("dir/to/nonexisting/file.lcm") == 2);
            }
        }

        std::remove("./test.lcm");
    }
}
