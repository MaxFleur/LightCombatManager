#include "UtilsGeneral.hpp"

#ifdef CATCH2_V3
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

TEST_CASE("Utils General Testing", "[UtilsGeneral]") {
    SECTION("CSV file path test") {
        SECTION("Example Latin") {
            REQUIRE(Utils::General::getLCMName("a/path/to/an/exampleTable.csv") == "exampleTable.csv");
        }
        SECTION("Example Umlaut") {
            REQUIRE(Utils::General::getLCMName("/rändöm/päth/tö/exämpleTäble.csv") == "exämpleTäble.csv");
        }
        SECTION("Example Cyrillic") {
            REQUIRE(Utils::General::getLCMName("путь/к/примеру.csv") == "примеру.csv");
        }
    }
}
