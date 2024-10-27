#include "AdditionalSettings.hpp"

#include <QSettings>

AdditionalSettings::AdditionalSettings()
{
    read();
}


void
AdditionalSettings::write(bool newIndicatorMultipleChars,
                          bool newRollIniMultipleChars,
                          bool newModAddedToIni)
{
    QSettings settings;

    settings.beginGroup("additional");
    writeParameter(settings, newIndicatorMultipleChars, indicatorMultipleChars, "indicator_multiple_chars");
    writeParameter(settings, newRollIniMultipleChars, rollIniMultipleChars, "roll_ini_for_multiple_chars");
    writeParameter(settings, newModAddedToIni, modAddedToIni, "mod_added_to_ini");
    settings.endGroup();
}


void
AdditionalSettings::read()
{
    QSettings settings;

    settings.beginGroup("additional");
    indicatorMultipleChars = settings.value("indicator_multiple_chars").isValid() ?
                             settings.value("indicator_multiple_chars").toBool() :
                             true;
    rollIniMultipleChars = settings.value("roll_ini_for_multiple_chars").isValid() ?
                           settings.value("roll_ini_for_multiple_chars").toBool() :
                           false;
    modAddedToIni = settings.value("mod_added_to_ini").isValid() ?
                    settings.value("mod_added_to_ini").toBool() :
                    true;
    settings.endGroup();
}
