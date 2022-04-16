#include "SettingsData.hpp"

#include <QSettings>

SettingsData::SettingsData()
{
	readSettings();
}


void
SettingsData::readSettings()
{
	QSettings settings;

	settings.beginGroup("RuleSettings");

	m_ruleset = settings.value("ruleset").isValid() ?
		    static_cast<Ruleset>(settings.value("ruleset").toInt()) :
		    Ruleset::PATHFINDER_1E_DND_3_5E;
	m_rollAutomatically = settings.value("roll_auto").isValid() ?
			      settings.value("roll_auto").toBool() : true;
	settings.endGroup();
}


void
SettingsData::writeSettings(int ruleset, bool rollAutomatically)
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
