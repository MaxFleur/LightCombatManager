#include "TableSettings.hpp"

#include <QSettings>

TableSettings::TableSettings()
{
    read();
}


void
TableSettings::write(ValueType valueType, bool valueToWrite)
{
    QSettings settings;

    settings.beginGroup("table");
    switch (valueType) {
    case ValueType::INI_SHOWN:
        writeParameter(settings, valueToWrite, iniShown, "ini");
        break;
    case ValueType::MOD_SHOWN:
        writeParameter(settings, valueToWrite, modifierShown, "modifier");
        break;
    case ValueType::COLOR_TABLE:
        writeParameter(settings, valueToWrite, colorTableRows, "color_rows");
        break;
    case ValueType::SHOW_INI_TOOLTIPS:
        writeParameter(settings, valueToWrite, showIniToolTips, "ini_tool_tips");
    default:
        break;
    }

    settings.endGroup();
}


void
TableSettings::read()
{
    QSettings settings;

    settings.beginGroup("table");
    iniShown = settings.value("ini").isValid() ? settings.value("ini").toBool() : true;
    modifierShown = settings.value("modifier").isValid() ? settings.value("modifier").toBool() : true;
    colorTableRows = settings.value("color_rows").isValid() ? settings.value("color_rows").toBool() : false;
    showIniToolTips = settings.value("ini_tool_tips").isValid() ? settings.value("ini_tool_tips").toBool() : false;
    settings.endGroup();
}
