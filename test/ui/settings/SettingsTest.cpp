#include "AdditionalSettings.hpp"
#include "DirSettings.hpp"
#include "RuleSettings.hpp"
#include "TableSettings.hpp"

#ifdef CATCH2_V3
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif

#include <QSettings>

TEST_CASE("Settings Testing", "[Settings]") {
    SECTION("Concepts test") {
        enum TestEnum {};
        struct TestStruct {};

        REQUIRE(SettingsValue<int>);
        REQUIRE(SettingsValue<bool>);
        REQUIRE(SettingsValue<QString>);
        REQUIRE(SettingsValue<TestEnum>);
        REQUIRE(!SettingsValue<float>);
        REQUIRE(!SettingsValue<char>);
        REQUIRE(!SettingsValue<std::string>);
        REQUIRE(!SettingsValue<TestStruct>);
    }

    SECTION("Additional settings test") {
        AdditionalSettings additionalSettings;
        QSettings settings;
        settings.clear();

        settings.beginGroup("additional");
        REQUIRE(settings.value("indicator_multiple_chars").isValid() == false);
        REQUIRE(settings.value("roll_ini_for_multiple_chars").isValid() == false);
        REQUIRE(settings.value("mod_added_to_ini").isValid() == false);
        settings.endGroup();

        additionalSettings.write(false, true, false);
        settings.beginGroup("additional");
        REQUIRE(settings.value("indicator_multiple_chars").isValid() == true);
        REQUIRE(settings.value("roll_ini_for_multiple_chars").isValid() == true);
        REQUIRE(settings.value("mod_added_to_ini").isValid() == true);
        REQUIRE(settings.value("indicator_multiple_chars").toBool() == false);
        REQUIRE(settings.value("roll_ini_for_multiple_chars").toBool() == true);
        REQUIRE(settings.value("mod_added_to_ini").toBool() == false);
        settings.endGroup();

        additionalSettings.write(true, false, true);
        settings.beginGroup("additional");
        REQUIRE(settings.value("indicator_multiple_chars").toBool() == true);
        REQUIRE(settings.value("roll_ini_for_multiple_chars").toBool() == false);
        REQUIRE(settings.value("mod_added_to_ini").toBool() == true);
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

        settings.beginGroup("rules");
        REQUIRE(settings.value("ruleset").isValid() == false);
        REQUIRE(settings.value("roll_auto").isValid() == false);
        settings.endGroup();

        ruleSettings.write(RuleSettings::Ruleset::DND_5E, true);
        settings.beginGroup("rules");
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

        settings.beginGroup("table");
        REQUIRE(settings.value("ini").isValid() == false);
        REQUIRE(settings.value("modifier").isValid() == false);
        REQUIRE(settings.value("color_rows").isValid() == false);
        REQUIRE(settings.value("ini_tool_tips").isValid() == false);
        REQUIRE(settings.value("adjust_height_remove").isValid() == false);
        settings.endGroup();

        tableSettings.write(TableSettings::ValueType::INI_SHOWN, false);
        tableSettings.write(TableSettings::ValueType::MOD_SHOWN, false);
        tableSettings.write(TableSettings::ValueType::COLOR_TABLE, true);
        tableSettings.write(TableSettings::ValueType::SHOW_INI_TOOLTIPS, true);
        tableSettings.write(TableSettings::ValueType::ADJUST_HEIGHT_AFTER_REMOVE, true);

        settings.beginGroup("table");
        REQUIRE(settings.value("ini").isValid() == true);
        REQUIRE(settings.value("modifier").isValid() == true);
        REQUIRE(settings.value("color_rows").isValid() == true);
        REQUIRE(settings.value("ini_tool_tips").isValid() == true);
        REQUIRE(settings.value("adjust_height_remove").isValid() == true);

        REQUIRE(settings.value("ini").toBool() == false);
        REQUIRE(settings.value("modifier").toBool() == false);
        REQUIRE(settings.value("color_rows").toBool() == true);
        REQUIRE(settings.value("ini_tool_tips").toBool() == true);
        REQUIRE(settings.value("adjust_height_remove").toBool() == true);
        settings.endGroup();
        settings.clear();
    }
}
