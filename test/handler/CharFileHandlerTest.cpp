#include "AdditionalInfoData.hpp"
#include "AdditionalInfoWidget.hpp"
#include "CharacterHandler.hpp"
#include "CombatTableWidget.hpp"
#include "CharFileHandler.hpp"
#include <catch2/catch.hpp>

#include <QFile>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QWidget>

#include <filesystem>

class CharFileHandlerTestUtils
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


TEST_CASE_METHOD(CharFileHandlerTestUtils, "CharFileHandler Testing", "[CharFileHandler]") {
    auto const fileHandler = std::make_shared<CharFileHandler>();
    auto const dirSettings = fileHandler->getDirSettings();
    auto testChar = "test.char";
    auto brokenChar = "broken.char";
    //save to default directory if no directory is set
    if (dirSettings.saveDir.isEmpty()) {
        testChar = "./test.char";
        brokenChar = "./broken.char";
    }
    SECTION("Check file saving") {
        CharacterHandler::Character character = CharacterHandler::Character( "test",
                                                                             0,
                                                                             0,
                                                                             10,
                                                                            false,
                                                                             AdditionalInfoData{ {}, "Haste" });
                const auto tableSaved = fileHandler->writeToFile(character, testChar);

        SECTION("Char successfully saved") {
            REQUIRE(tableSaved == true);
        }

        SECTION("File format and content correct") {
            const auto codeCSVStatus = fileHandler->getStatus(testChar);
            REQUIRE(codeCSVStatus == 0);

            const auto& jsonObject = fileHandler->getData();
            REQUIRE(jsonObject.value("name").toString() == "test");
            REQUIRE(jsonObject.value("initiative").toInt() == 0);
            REQUIRE(jsonObject.value("modifier").toInt() == 0);
            REQUIRE(jsonObject.value("hp").toInt() == 10);
            REQUIRE(jsonObject.value("is_enemy").toBool() == false);
            REQUIRE(jsonObject.value("additional_info").toString() == "Haste");
        }
        SECTION("Check format test") {
            SECTION("Functioning Char") {
                REQUIRE(fileHandler->getStatus(resolvePath(testChar)) == 0);
            }
            SECTION("Broken Char") {
                // Incomplete json object
                QJsonObject jsonObject;
                jsonObject["modifier"] = 2;
                jsonObject["initiative"] = 3;

                // Write to file
                auto byteArray = QJsonDocument(jsonObject).toJson();
                QFile fileOut("broken.char");
                fileOut.open(QIODevice::WriteOnly);

                fileOut.write(byteArray);
                REQUIRE(fileHandler->getStatus(resolvePath(brokenChar)) == 1);
            }
            SECTION("Dir instead of filename") {
                REQUIRE(fileHandler->getStatus("dir/to/nonexisting/file.char") == 2);
            }
            SECTION("File does not exist") {
                REQUIRE(fileHandler->getStatus("noexist.char") == 2);
            }
        }

        SECTION("Check file removal") {
            SECTION("File removed unsuccessfully") {
                const auto fileRemoved = fileHandler->removeCharacter("noexist.char");
                REQUIRE(fileRemoved == false);
            }
            SECTION("File removed successfully"){
                const auto fileRemoved = fileHandler->removeCharacter(testChar);
                REQUIRE(fileRemoved == true);

                // Check if file no longer exists
                REQUIRE(!std::filesystem::exists(testChar) ==true);

            }
            QFile CharfileOut(dirSettings.saveDir  + testChar);
            if (CharfileOut.exists()) {
                CharfileOut.remove();
            }
            QFile BrokenfileOut(dirSettings.saveDir  + brokenChar);
            if (BrokenfileOut.exists()) {
                BrokenfileOut.remove();
            }
        }
    }
}
