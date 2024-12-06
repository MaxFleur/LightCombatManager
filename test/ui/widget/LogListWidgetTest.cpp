#include "LogListWidget.hpp"

#ifdef CATCH2_V3
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

using enum LogListWidget::LoggingType;

TEST_CASE("Logging List Widget Testing", "[TableUtils]") {
    auto* const logListWidget = new LogListWidget;

    SECTION("Log single value test") {
        logListWidget->logSingleValue(NAME, 0);
        logListWidget->logSingleValue(INI, 0);
        logListWidget->logSingleValue(INI_MOD, 0);
        logListWidget->logSingleValue(HP, 0);
        logListWidget->logSingleValue(INFO_TEXT, 0);
        logListWidget->logSingleValue(DUPLICATE, 0);
        logListWidget->logSingleValue(INFO_EFFECT, 0);

        REQUIRE(logListWidget->count() == 7);
        REQUIRE(logListWidget->item(6)->text() == "Changed name for row 0.");
        REQUIRE(logListWidget->item(5)->text() == "Set INI for row 0.");
        REQUIRE(logListWidget->item(4)->text() == "Set INI modifier for row 0.");
        REQUIRE(logListWidget->item(3)->text() == "Set HP for row 0.");
        REQUIRE(logListWidget->item(2)->text() == "Added more info for row 0.");
        REQUIRE(logListWidget->item(1)->text() == "Duplicated row 0.");
        REQUIRE(logListWidget->item(0)->text() == "Modified effects for 0 Characters.");
    }
    SECTION("Log conditional value test") {
        logListWidget->logConditionalValue(ALLY, 0, true);
        logListWidget->logConditionalValue(ALLY, 0, false);
        logListWidget->logConditionalValue(COUNT, 2, true);
        logListWidget->logConditionalValue(COUNT, 2, false);
        logListWidget->logConditionalValue(MULTIPLE_CHARS, 3, true);
        logListWidget->logConditionalValue(MULTIPLE_CHARS, 3, false);

        REQUIRE(logListWidget->count() == 6);
        REQUIRE(logListWidget->item(5)->text() == "Set enemy status in row 0 to 'Ally'.");
        REQUIRE(logListWidget->item(4)->text() == "Set enemy status in row 0 to 'Enemy'.");
        REQUIRE(logListWidget->item(3)->text() == "Added 2 Character(s).");
        REQUIRE(logListWidget->item(2)->text() == "Removed 2 Character(s).");
        REQUIRE(logListWidget->item(1)->text() == "Changed HP for 3 Characters.");
        REQUIRE(logListWidget->item(0)->text() == "Changed Info Text for 3 Characters.");
    }
    SECTION("Log two values test") {
        logListWidget->logTwoValues(SWITCH, 0, 1);
        logListWidget->logTwoValues(NEXT_TURN, 1, 0);

        REQUIRE(logListWidget->count() == 2);
        REQUIRE(logListWidget->item(1)->text() == "Switched row 0 and 1.");
        REQUIRE(logListWidget->item(0)->text() == "Next turn (round 1, row 0).");
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
