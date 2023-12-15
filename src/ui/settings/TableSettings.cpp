#include "TableSettings.hpp"

#include <QSettings>

TableSettings::TableSettings()
{
    read();
}


void
TableSettings::write(bool valueToWrite, ValueType valueType)
{
    QSettings settings;

    settings.beginGroup("TableSettings");
    switch (valueType) {
    case ValueType::INI_SHOWN:
        iniShown = valueToWrite;
        settings.setValue("INI", iniShown);
        break;
    case ValueType::MOD_SHOWN:
        modifierShown = valueToWrite;
        settings.setValue("Modifier", modifierShown);
        break;
    case ValueType::COLOR_TABLE:
        colorTableRows = valueToWrite;
        settings.setValue("ColorTable", colorTableRows);
        break;
    case ValueType::SHOW_INI_TOOLTIPS:
        showIniToolTips = valueToWrite;
        settings.setValue("IniToolTips", showIniToolTips);
    default:
        break;
    }

    settings.endGroup();
}


void
TableSettings::read()
{
    QSettings settings;

    settings.beginGroup("TableSettings");
    iniShown = settings.value("INI").isValid() ? settings.value("INI").toBool() : true;
    modifierShown = settings.value("Modifier").isValid() ? settings.value("Modifier").toBool() : true;
    colorTableRows = settings.value("ColorTable").isValid() ? settings.value("ColorTable").toBool() : false;
    showIniToolTips = settings.value("IniToolTips").isValid() ? settings.value("IniToolTips").toBool() : false;
    settings.endGroup();
}
