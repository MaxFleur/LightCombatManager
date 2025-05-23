#include "AdditionalInfoData.hpp"
#include "CharacterHandler.hpp"
#include "CharFileHandler.hpp"

#ifdef CATCH2_V3
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include <QDir>

TEST_CASE("CharFileHandler Testing", "[CharFileHandler]") {
    auto const charFileHandler = std::make_shared<CharFileHandler>();
    const auto character = CharacterHandler::Character("test", 0, 0, 10, false, AdditionalInfoData{ .mainInfoText = "Haste" });
    const auto tableSaved = charFileHandler->writeToFile(character);

    QDir dir;
    const auto charPath = dir.currentPath() + "/chars/test.char";

    SECTION("Char successfully saved") {
        REQUIRE(tableSaved == true);
        REQUIRE(dir.exists(charPath));
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
        charFileHandler->writeJsonObjectToFile(jsonObject, dir.currentPath() + "/chars/broken.char");

        REQUIRE(charFileHandler->getStatus("broken.char") == 1);
        dir.remove(dir.currentPath() + "/chars/broken.char");
    }
    SECTION("Not existing file") {
        const auto codeCSVStatus = charFileHandler->getStatus("nonexisting.char");
        REQUIRE(codeCSVStatus == 2);
    }

    std::remove("./chars/test.char");
}
