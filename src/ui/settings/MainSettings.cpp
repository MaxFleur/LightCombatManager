#include "MainSettings.hpp"

#include <QSettings>

MainSettings::MainSettings()
{
	read();
}


void
MainSettings::read()
{
	QSettings settings;

	settings.beginGroup("RuleSettings");

	ruleset = settings.value("ruleset").isValid() ?
		    static_cast<Ruleset>(settings.value("ruleset").toInt()) :
		    Ruleset::PATHFINDER_1E_DND_3_5E;
	rollAutomatical = settings.value("roll_auto").isValid() ?
			      settings.value("roll_auto").toBool() : false;
	settings.endGroup();
}


void
MainSettings::write(int newRuleset, bool newRollAutomatical)
{
	QSettings settings;

	settings.beginGroup("RuleSettings");
	if (ruleset != newRuleset) {
		ruleset = static_cast<Ruleset>(newRuleset);
		settings.setValue("ruleset", ruleset);
	}
	if (rollAutomatical != newRollAutomatical) {
		rollAutomatical = newRollAutomatical;
		settings.setValue("roll_auto", rollAutomatical);
	}
	settings.endGroup();
}
