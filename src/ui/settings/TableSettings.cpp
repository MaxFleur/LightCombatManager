#include "TableSettings.hpp"

#include <QSettings>

TableSettings::TableSettings()
{
    read();
}


void
TableSettings::write(bool value, ValueType valueType)
{
    QSettings settings;

    settings.beginGroup("TableSettings");
    switch (valueType) {
    case INI_SHOWN:
        iniShown = value;
        settings.setValue("INI", iniShown);
        break;
    case MOD_SHOWN:
        modifierShown = value;
        settings.setValue("Modifier", modifierShown);
        break;
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
    settings.endGroup();
}
