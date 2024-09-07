#include "AdditionalSettings.hpp"
#include "DirSettings.hpp"
#include "RuleSettings.hpp"
#include "TableSettings.hpp"

#include <catch2/catch_test_macros.hpp>

#include <QSettings>

TEST_CASE("Settings Testing", "[Settings]") {
    SECTION("Additional settings test") {
        AdditionalSettings additionalSettings;
        QSettings settings;
        settings.clear();

        settings.beginGroup("AdditionalSettings");
        REQUIRE(settings.value("indicatorMultipleChars").isValid() == false);
        REQUIRE(settings.value("rollIniMultipleChars").isValid() == false);
        REQUIRE(settings.value("modAddedToIni").isValid() == false);
        settings.endGroup();

        additionalSettings.write(false, true, false);
        settings.beginGroup("AdditionalSettings");
        REQUIRE(settings.value("indicatorMultipleChars").isValid() == true);
        REQUIRE(settings.value("rollIniMultipleChars").isValid() == true);
        REQUIRE(settings.value("modAddedToIni").isValid() == true);
        REQUIRE(settings.value("indicatorMultipleChars").toBool() == false);
        REQUIRE(settings.value("rollIniMultipleChars").toBool() == true);
        REQUIRE(settings.value("modAddedToIni").toBool() == false);
        settings.endGroup();

        additionalSettings.write(true, false, true);
        settings.beginGroup("AdditionalSettings");
        REQUIRE(settings.value("indicatorMultipleChars").toBool() == true);
        REQUIRE(settings.value("rollIniMultipleChars").toBool() == false);
        REQUIRE(settings.value("modAddedToIni").toBool() == true);
        settings.endGroup();
    }

    SECTION("Dir settings test") {
        DirSettings dirSettings;
        QSettings settings;
        settings.clear();

        REQUIRE(settings.value("dir_save").isValid() == false);
        REQUIRE(settings.value("dir_open").isValid() == false);

        dirSettings.write("/example/path/dir_open_and_save", true);
        REQUIRE(settings.value("dir_save").isValid() == true);
        REQUIRE(settings.value("dir_open").isValid() == true);
        REQUIRE(settings.value("dir_open").toString() == "/example/path/dir_open_and_save");
        REQUIRE(settings.value("dir_save").toString() == "/example/path/dir_open_and_save");

        dirSettings.write("/example/path/new_path", false);
        REQUIRE(settings.value("dir_open").toString() == "/example/path/new_path");
        REQUIRE(settings.value("dir_save").toString() == "/example/path/dir_open_and_save");
    }
    SECTION("Rule settings test") {
        RuleSettings ruleSettings;
        QSettings settings;
        settings.clear();

        settings.beginGroup("RuleSettings");
        REQUIRE(settings.value("ruleset").isValid() == false);
        REQUIRE(settings.value("roll_auto").isValid() == false);
        settings.endGroup();

        ruleSettings.write(2, true);
        settings.beginGroup("RuleSettings");
        REQUIRE(settings.value("ruleset").isValid() == true);
        REQUIRE(settings.value("roll_auto").isValid() == true);
        REQUIRE(static_cast<RuleSettings::Ruleset>(settings.value("ruleset").toInt()) == RuleSettings::Ruleset::DND_5E);
        REQUIRE(settings.value("roll_auto").toBool() == true);
        settings.endGroup();
    }

    SECTION("Table settings test") {
        TableSettings tableSettings;
        QSettings settings;
        settings.clear();

        settings.beginGroup("TableSettings");
        REQUIRE(settings.value("INI").isValid() == false);
        REQUIRE(settings.value("Modifier").isValid() == false);
        REQUIRE(settings.value("ColorTable").isValid() == false);
        REQUIRE(settings.value("IniToolTips").isValid() == false);
        settings.endGroup();

        tableSettings.write(TableSettings::ValueType::INI_SHOWN, true);
        tableSettings.write(TableSettings::ValueType::MOD_SHOWN, true);
        tableSettings.write(TableSettings::ValueType::COLOR_TABLE, false);
        tableSettings.write(TableSettings::ValueType::SHOW_INI_TOOLTIPS, false);

        settings.beginGroup("TableSettings");
        REQUIRE(settings.value("INI").isValid() == true);
        REQUIRE(settings.value("Modifier").isValid() == true);
        REQUIRE(settings.value("ColorTable").isValid() == true);
        REQUIRE(settings.value("IniToolTips").isValid() == true);

        REQUIRE(settings.value("INI").toBool() == true);
        REQUIRE(settings.value("Modifier").toBool() == true);
        REQUIRE(settings.value("ColorTable").toBool() == false);
        REQUIRE(settings.value("IniToolTips").toBool() == false);
        settings.endGroup();
    }
}
