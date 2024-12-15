#include "UtilsFiles.hpp"

#ifdef CATCH2_V3
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include <fstream>

TEST_CASE("Utils Files Testing", "[UtilsFiles]") {
    SECTION("Remove file test") {
        std::ofstream file;
        file.open("existing_file.txt");
        file.close();

        auto fileRemoved = Utils::Files::removeFile("existing_file.txt");
        REQUIRE(fileRemoved == false);

        fileRemoved = Utils::Files::removeFile("nonexisting_file.txt");
        REQUIRE(fileRemoved == false);
    }
}
