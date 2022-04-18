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

	m_ruleset = settings.value("ruleset").isValid() ?
		    static_cast<Ruleset>(settings.value("ruleset").toInt()) :
		    Ruleset::PATHFINDER_1E_DND_3_5E;
	m_rollAutomatically = settings.value("roll_auto").isValid() ?
			      settings.value("roll_auto").toBool() : false;
	settings.endGroup();
}


void
MainSettings::write(int ruleset, bool rollAutomatically)
{
	QSettings settings;

	settings.beginGroup("RuleSettings");
	if (m_ruleset != ruleset) {
		m_ruleset = static_cast<Ruleset>(ruleset);
		settings.setValue("ruleset", m_ruleset);
	}
	if (m_rollAutomatically != rollAutomatically) {
		m_rollAutomatically = rollAutomatically;
		settings.setValue("roll_auto", m_rollAutomatically);
	}
	settings.endGroup();
}
