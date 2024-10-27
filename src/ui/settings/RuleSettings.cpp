#include "RuleSettings.hpp"

#include <QSettings>

RuleSettings::RuleSettings()
{
    read();
}


void
RuleSettings::write(Ruleset newRuleset, bool newRollAutomatical)
{
    QSettings settings;

    settings.beginGroup("rules");
    writeParameter(settings, newRuleset, ruleset, "ruleset");
    writeParameter(settings, newRollAutomatical, rollAutomatical, "roll_auto");
    settings.endGroup();
}


void
RuleSettings::read()
{
    QSettings settings;

    settings.beginGroup("rules");
    ruleset = settings.value("ruleset").isValid() ?
              static_cast<Ruleset>(settings.value("ruleset").toInt()) :
              Ruleset::PATHFINDER_1E_DND_35E;
    rollAutomatical = settings.value("roll_auto").isValid() ?
                      settings.value("roll_auto").toBool() : false;
    settings.endGroup();
}
