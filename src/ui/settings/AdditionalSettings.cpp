#include "AdditionalSettings.hpp"

#include <QSettings>

AdditionalSettings::AdditionalSettings()
{
    read();
}


void
AdditionalSettings::write(bool newIndicatorForMultipleChars,
                          bool newRollIniForMultipleChars)
{
    QSettings settings;

    settings.beginGroup("AdditionalSettings");
    if (indicatorForMultipleChars != newIndicatorForMultipleChars) {
        indicatorForMultipleChars = newIndicatorForMultipleChars;
        settings.setValue("indicatorForMultipleChars", indicatorForMultipleChars);
    }
    if (rollIniForMultipleChars != newRollIniForMultipleChars) {
        rollIniForMultipleChars = newRollIniForMultipleChars;
        settings.setValue("rollIniForMultipleChars", newRollIniForMultipleChars);
    }
    settings.endGroup();
}


void
AdditionalSettings::read()
{
    QSettings settings;

    settings.beginGroup("AdditionalSettings");
    indicatorForMultipleChars = settings.value("indicatorForMultipleChars").isValid() ?
                                settings.value("indicatorForMultipleChars").toBool() :
                                true;
    rollIniForMultipleChars = settings.value("rollIniForMultipleChars").isValid() ?
                              settings.value("rollIniForMultipleChars").toBool() :
                              false;
    settings.endGroup();
}
