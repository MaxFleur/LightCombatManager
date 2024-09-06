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

    settings.beginGroup("AdditionalSettings");
    if (indicatorMultipleChars != newIndicatorMultipleChars) {
        indicatorMultipleChars = newIndicatorMultipleChars;
        settings.setValue("indicatorMultipleChars", indicatorMultipleChars);
    }
    if (rollIniMultipleChars != newRollIniMultipleChars) {
        rollIniMultipleChars = newRollIniMultipleChars;
        settings.setValue("rollIniMultipleChars", rollIniMultipleChars);
    }
    if (modAddedToIni != newModAddedToIni) {
        modAddedToIni = newModAddedToIni;
        settings.setValue("modAddedToIni", modAddedToIni);
    }
    settings.endGroup();
}


void
AdditionalSettings::read()
{
    QSettings settings;

    settings.beginGroup("AdditionalSettings");
    indicatorMultipleChars = settings.value("indicatorMultipleChars").isValid() ?
                             settings.value("indicatorMultipleChars").toBool() :
                             true;
    rollIniMultipleChars = settings.value("rollIniMultipleChars").isValid() ?
                           settings.value("rollIniMultipleChars").toBool() :
                           false;
    modAddedToIni = settings.value("modAddedToIni").isValid() ?
                    settings.value("modAddedToIni").toBool() :
                    true;
    settings.endGroup();
}
