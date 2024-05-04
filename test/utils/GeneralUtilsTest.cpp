#include "AdditionalInfoWidget.hpp"
#include "UtilsGeneral.hpp"

#include <catch2/catch.hpp>

#include <QHBoxLayout>

TEST_CASE("General Util Testing", "[GeneralUtils]") {
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
