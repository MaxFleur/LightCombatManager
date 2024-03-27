#include "AdditionalInfoData.hpp"
#include "CharacterHandler.hpp"
#include "CharFileHandler.hpp"

#include <catch2/catch.hpp>

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QSettings>

#include <filesystem>

TEST_CASE("CharFileHandler Testing", "[CharFileHandler]") {
    auto const charFileHandler = std::make_shared<CharFileHandler>();
    const auto character = CharacterHandler::Character("test", 0, 0, 10, false, AdditionalInfoData{ {}, "Haste" });
    const auto tableSaved = charFileHandler->writeToFile(character);

    QSettings settings;
    QDir dir(settings.fileName());
    dir.cdUp();
    const auto charPath = dir.absolutePath() + "/test.char";

    SECTION("Char successfully saved") {
        REQUIRE(tableSaved == true);
        REQUIRE(std::filesystem::exists(charPath.toStdString()));
    }
    SECTION("File format and content correct") {
        const auto codeCSVStatus = charFileHandler->getStatus("test.char");
        REQUIRE(codeCSVStatus == 0);

        const auto& jsonObject = charFileHandler->getData();
        REQUIRE(jsonObject.value("name").toString() == "test");
        REQUIRE(jsonObject.value("initiative").toInt() == 0);
        REQUIRE(jsonObject.value("modifier").toInt() == 0);
        REQUIRE(jsonObject.value("hp").toInt() == 10);
        REQUIRE(jsonObject.value("is_enemy").toBool() == false);
        REQUIRE(jsonObject.value("additional_info").toString() == "Haste");
    }

    SECTION("Broken table") {
        // Incomplete json object
        QJsonObject jsonObject;
        jsonObject["name"] = 2;

        // Write to file
        auto byteArray = QJsonDocument(jsonObject).toJson();
        QFile fileOut(dir.absolutePath() + "/broken.char");
        fileOut.open(QIODevice::WriteOnly);
        fileOut.write(byteArray);

        REQUIRE(charFileHandler->getStatus("broken.char") == 1);
        std::filesystem::remove(dir.absolutePath().toStdString() + "/broken.char");
    }
    SECTION("Not existing file") {
        const auto codeCSVStatus = charFileHandler->getStatus("nonexisting.char");
        REQUIRE(codeCSVStatus == 2);
    }
    SECTION("Check file removal") {
        const auto fileRemoved = charFileHandler->removeCharacter("test.char");
        REQUIRE(fileRemoved == true);
        REQUIRE(!std::filesystem::exists(charPath.toStdString()));
    }
}
