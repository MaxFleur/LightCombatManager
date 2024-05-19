#include "CharacterHandler.hpp"
#include "TemplatesListWidget.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Templates List Widget Testing", "[TableUtils]") {
    auto* const templatesListWidget = new TemplatesListWidget;
    const auto character = CharacterHandler::Character("test", 0, -3, 10, false, AdditionalInfoData{ {}, "Haste" });
    const auto characterAdded = templatesListWidget->addCharacter(character);

    SECTION("Added character storage tests") {
        REQUIRE(characterAdded == true);
        REQUIRE(templatesListWidget->count() == 1);

        auto* const item = templatesListWidget->item(0);
        REQUIRE(item->text() == "test");

        const auto character = item->data(Qt::UserRole).value<CharacterHandler::Character>();
        REQUIRE(character.name == "test");
        REQUIRE(character.initiative == 0);
        REQUIRE(character.modifier == -3);
        REQUIRE(character.hp == 10);
        REQUIRE(character.isEnemy == false);
        REQUIRE(character.additionalInfoData.mainInfoText == "Haste");
    }
    SECTION("Same character again") {
        const auto sameCharacterAddedAgain = templatesListWidget->addCharacter(character);
        REQUIRE(sameCharacterAddedAgain == false);
    }
    SECTION("Remove character test") {
        const auto characterRemoved = templatesListWidget->removeCharacter(character);
        REQUIRE(characterRemoved == true);
        REQUIRE(templatesListWidget->count() == 0);
    }
    SECTION("Remove another unadded character test") {
        const auto anotherCharacter = CharacterHandler::Character("test2", 0, -3, 10, false, AdditionalInfoData{ {}, "Haste" });
        const auto characterRemoved = templatesListWidget->removeCharacter(anotherCharacter);
        REQUIRE(characterRemoved == false);
    }
}
