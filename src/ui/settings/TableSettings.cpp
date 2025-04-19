#include "TableSettings.hpp"

#include <QSettings>

TableSettings::TableSettings()
{
    read();
}


void
TableSettings::write(ValueType valueType, bool valueToWrite)
{
    using enum ValueType;
    QSettings settings;

    settings.beginGroup("table");
    switch (valueType) {
    case INI_SHOWN:
        writeParameter(settings, valueToWrite, iniShown, "ini");
        break;
    case MOD_SHOWN:
        writeParameter(settings, valueToWrite, modifierShown, "modifier");
        break;
    case COLOR_TABLE:
        writeParameter(settings, valueToWrite, colorTableRows, "color_rows");
        break;
    case SHOW_INI_TOOLTIPS:
        writeParameter(settings, valueToWrite, showIniToolTips, "ini_tool_tips");
        break;
    case ADJUST_HEIGHT_AFTER_REMOVE:
        writeParameter(settings, valueToWrite, adjustHeightAfterRemove, "adjust_height_remove");
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
    adjustHeightAfterRemove = settings.value("adjust_height_remove").isValid() ? settings.value("adjust_height_remove").toBool() : false;
    settings.endGroup();
}
