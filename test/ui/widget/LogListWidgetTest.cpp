#include "LogListWidget.hpp"

#ifdef CATCH2_V3
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

TEST_CASE("Logging List Widget Testing", "[TableUtils]") {
    auto* const logListWidget = new LogListWidget;

    SECTION("Log single value changes") {
        logListWidget->log(LogListWidget::LogType::CHANGED_MULTIPLE_HP, 2);
        logListWidget->log(LogListWidget::LogType::DUPLICATED_CHAR, 0);
        logListWidget->log(LogListWidget::LogType::ADDED_EFFECT, 1);

        REQUIRE(logListWidget->count() == 3);
        REQUIRE(logListWidget->item(0)->text() == "Modified effects for 1 Character(s).");
        REQUIRE(logListWidget->item(1)->text() == "Duplicated row 0.");
        REQUIRE(logListWidget->item(2)->text() == "Changed HP for 2 Character(s).");
    }

    SECTION("Log double value changes") {
        logListWidget->log(LogListWidget::LogType::CHANGED_CHAR_COUNT, 1, 1);
        logListWidget->log(LogListWidget::LogType::CHANGED_CHAR_COUNT, 1, 0);
        REQUIRE(logListWidget->count() == 2);
        REQUIRE(logListWidget->item(0)->text() == "Removed 1 Character(s).");
        REQUIRE(logListWidget->item(1)->text() == "Added 1 Character(s).");

        logListWidget->log(LogListWidget::LogType::CHARS_SWITCHED, 2, 3);
        logListWidget->log(LogListWidget::LogType::NEXT_TURN, 1, 2);
        REQUIRE(logListWidget->count() == 4);
        REQUIRE(logListWidget->item(0)->text() == "Next turn (round 1, row 2).");
        REQUIRE(logListWidget->item(1)->text() == "Switched row 2 and 3.");
        REQUIRE(logListWidget->item(2)->text() == "Removed 1 Character(s).");
        REQUIRE(logListWidget->item(3)->text() == "Added 1 Character(s).");
    }

    SECTION("Log changed stat test") {
        logListWidget->log(LogListWidget::LogType::STAT_CHANGED, 0, 0);
        logListWidget->log(LogListWidget::LogType::STAT_CHANGED, 0, 1);
        logListWidget->log(LogListWidget::LogType::STAT_CHANGED, 0, 2);
        logListWidget->log(LogListWidget::LogType::STAT_CHANGED, 0, 3);
        logListWidget->log(LogListWidget::LogType::STAT_CHANGED, 0, 4, true);
        logListWidget->log(LogListWidget::LogType::STAT_CHANGED, 0, 4, false);
        logListWidget->log(LogListWidget::LogType::STAT_CHANGED, 0, 5);

        REQUIRE(logListWidget->count() == 7);
        REQUIRE(logListWidget->item(0)->text() == "Added more info for row 0.");
        REQUIRE(logListWidget->item(1)->text() == "Set enemy status in row 0 to 'Enemy'.");
        REQUIRE(logListWidget->item(2)->text() == "Set enemy status in row 0 to 'Ally'.");
        REQUIRE(logListWidget->item(3)->text() == "Set HP for row 0.");
        REQUIRE(logListWidget->item(4)->text() == "Set INI modifier for row 0.");
        REQUIRE(logListWidget->item(5)->text() == "Set INI for row 0.");
        REQUIRE(logListWidget->item(6)->text() == "Changed name for row 0.");
    }

    SECTION("Log other test") {
        logListWidget->log("Test string");

        REQUIRE(logListWidget->count() == 1);
        REQUIRE(logListWidget->item(0)->text() == "Test string");
    }
}
