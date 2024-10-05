#include "LogListWidget.hpp"

#ifdef CATCH2_V3
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

TEST_CASE("Logging List Widget Testing", "[TableUtils]") {
    auto* const logListWidget = new LogListWidget;

    SECTION("Log changed stat test") {
        logListWidget->logCharacterStatChanged(0, 0);
        logListWidget->logCharacterStatChanged(0, 1);
        logListWidget->logCharacterStatChanged(0, 2);
        logListWidget->logCharacterStatChanged(0, 3);
        logListWidget->logCharacterStatChanged(0, 4, true);
        logListWidget->logCharacterStatChanged(0, 4, false);
        logListWidget->logCharacterStatChanged(0, 5);

        REQUIRE(logListWidget->count() == 7);
        REQUIRE(logListWidget->item(6)->text() == "Changed name for row 0.");
        REQUIRE(logListWidget->item(5)->text() == "Set INI for row 0.");
        REQUIRE(logListWidget->item(4)->text() == "Set INI modifier for row 0.");
        REQUIRE(logListWidget->item(3)->text() == "Set HP for row 0.");
        REQUIRE(logListWidget->item(2)->text() == "Set enemy status in row 0 to 'Ally'.");
        REQUIRE(logListWidget->item(1)->text() == "Set enemy status in row 0 to 'Enemy'.");
        REQUIRE(logListWidget->item(0)->text() == "Added more info for row 0.");
    }
    SECTION("Log changed character count test") {
        logListWidget->logChangedCharacterCount(2, true);
        logListWidget->logChangedCharacterCount(1, false);

        REQUIRE(logListWidget->count() == 2);
        REQUIRE(logListWidget->item(1)->text() == "Added 2 Character(s).");
        REQUIRE(logListWidget->item(0)->text() == "Removed 1 Character(s).");
    }
    SECTION("Log duplicated character test") {
        logListWidget->logCharacterDuplicated(2);

        REQUIRE(logListWidget->count() == 1);
        REQUIRE(logListWidget->item(0)->text() == "Duplicated row 2.");
    }
    SECTION("Log character switch test") {
        logListWidget->logCharacterSwitch(1, 2);

        REQUIRE(logListWidget->count() == 1);
        REQUIRE(logListWidget->item(0)->text() == "Switched row 1 and 2.");
    }
    SECTION("Log modified effects test") {
        logListWidget->logAddedEffects(2);

        REQUIRE(logListWidget->count() == 1);
        REQUIRE(logListWidget->item(0)->text() == "Modified effects for 2 Characters.");
    }
    SECTION("Log next turn test") {
        logListWidget->logNextTurn(2, 4);

        REQUIRE(logListWidget->count() == 1);
        REQUIRE(logListWidget->item(0)->text() == "Next turn (round 2, row 4).");
    }
    SECTION("Log other test") {
        logListWidget->logOther("Redo called.");
        logListWidget->logOther("Test text");
        logListWidget->logOther("Redo called.");

        REQUIRE(logListWidget->count() == 2);
        REQUIRE(logListWidget->item(1)->text() == "Test text");
        REQUIRE(logListWidget->item(0)->text() == "Redo called.");
    }
}
