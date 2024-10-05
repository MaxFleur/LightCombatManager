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
    }
}
