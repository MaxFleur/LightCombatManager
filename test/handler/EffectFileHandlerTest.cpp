#include "EffectFileHandler.hpp"

#ifdef CATCH2_V3
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include <QDir>

TEST_CASE("EffectFileHandler Testing", "[EffectFileHandler]") {
    auto const effectFileHandler = std::make_unique<EffectFileHandler>();
    const auto effectSaved = effectFileHandler->writeToFile("Test Effect");

    QDir dir;
    const auto effectPath = dir.currentPath() + "/effects/Test Effect.effect";

    SECTION("Effect successfully saved") {
        REQUIRE(effectSaved == true);
        REQUIRE(dir.exists(effectPath));
    }
    SECTION("File format and content correct") {
        const auto codeCSVStatus = effectFileHandler->getStatus("Test Effect.effect");
        REQUIRE(codeCSVStatus == 0);

        const auto& jsonObject = effectFileHandler->getData();
        REQUIRE(jsonObject.value("name").toString() == "Test Effect");
    }

    SECTION("Broken table") {
        // Incomplete json object
        QJsonObject jsonObject;
        jsonObject["broken"] = 2;
        // Write to file
        effectFileHandler->writeJsonObjectToFile(jsonObject, dir.currentPath() + "/effects/Broken Effect.effect");

        REQUIRE(effectFileHandler->getStatus("Broken Effect.effect") == 1);
        dir.remove(dir.currentPath() + "/effects/Broken Effect.effect");
    }
    SECTION("Non-existent file") {
        const auto codeCSVStatus = effectFileHandler->getStatus("nonexisting.effect");
        REQUIRE(codeCSVStatus == 2);
    }

    std::remove("./effects/Test Effect.effect");
}
