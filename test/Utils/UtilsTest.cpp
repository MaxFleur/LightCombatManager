#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <QDebug>
#include <QString>

#include "Utils.hpp"


TEST_CASE("Util Testing", "[Utils]"){
	SECTION("Enclose semicolons"){
		QString testString = "This; test; contains; semicolons;";
		qDebug() << Utils::replaceCharacters(testString, true);
		REQUIRE(
			Utils::replaceCharacters(
				testString,
				true) == "This\";\" test\";\" contains\";\" semicolons\";\"");
	}
	SECTION("Remove semicolons"){
		QString testString = "This\";\" test\";\" contains\";\" semicolons\";\"";
		REQUIRE(Utils::replaceCharacters(testString, true) == "This; test; contains; semicolons;");
	}
}
