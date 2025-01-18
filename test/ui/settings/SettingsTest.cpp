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

#include <filesystem>
#include <fstream>

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

        // Create file so that the settings entry won't be deleted
        std::filesystem::create_directories(std::filesystem::current_path().string() + "/example");
        std::ofstream file(std::filesystem::current_path().string() + "/example/test.lcm");
        file << "Text";
        file.close();
        const auto lcmFilePath = QString::fromStdString(std::filesystem::current_path().string() + "/example/test.lcm");

        REQUIRE(settings.value("dir_save").isValid() == false);
        REQUIRE(settings.value("dir_open").isValid() == false);
        REQUIRE(settings.value("recent_dir_0").isValid() == false);
        REQUIRE(settings.value("recent_dir_1").isValid() == false);

        dirSettings.write(lcmFilePath, true);
        REQUIRE(settings.value("dir_save").isValid() == true);
        REQUIRE(settings.value("dir_open").isValid() == true);
        REQUIRE(settings.value("recent_dir_0").isValid() == true);
        REQUIRE(settings.value("dir_open").toString() == lcmFilePath);
        REQUIRE(settings.value("dir_save").toString() == lcmFilePath);
        REQUIRE(settings.value("recent_dir_0").toString() == lcmFilePath);

        dirSettings.write("/example/invalid.csv", false);
        REQUIRE(settings.value("dir_open").toString() == "/example/invalid.csv");
        REQUIRE(settings.value("dir_save").toString() == lcmFilePath);
        REQUIRE(settings.value("recent_dir_1").isValid() == true);
        REQUIRE(settings.value("recent_dir_0").toString() == "/example/invalid.csv");
        REQUIRE(settings.value("recent_dir_1").toString() == lcmFilePath);

        DirSettings newDirSettings;
        REQUIRE(settings.value("dir_save").isValid() == true);
        REQUIRE(settings.value("dir_open").isValid() == true);
        // These files never really existed, therefore the settings key should have been deleted
        REQUIRE(settings.value("recent_dir_0").isValid() == false);
        REQUIRE(settings.value("recent_dir_1").isValid() == true);

        std::filesystem::remove_all("example/test.lcm");
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
