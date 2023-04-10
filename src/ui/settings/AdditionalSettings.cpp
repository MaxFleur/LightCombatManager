#include "AdditionalSettings.hpp"

#include <QSettings>

AdditionalSettings::AdditionalSettings()
{
    read();
}


void
AdditionalSettings::write(bool newindicatorForMultipleChars)
{
    QSettings settings;

    settings.beginGroup("AdditionalSettings");
    if (indicatorForMultipleChars != newindicatorForMultipleChars) {
        indicatorForMultipleChars = newindicatorForMultipleChars;
        settings.setValue("indicatorForMultipleChars", indicatorForMultipleChars);
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
    settings.endGroup();
}
