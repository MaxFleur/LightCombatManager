#pragma once

// Store data used for the settings dialog
class SettingsData {
public:

	SettingsData();

	enum Ruleset {
		PATHFINDER_1E_DND_3_5E	= 0,
		PATHFINDER_2E		= 1,
		DND_5E			= 2
	};

	void
	writeSettings(int	ruleset,
		      bool	rollAutomatically);

public:
	Ruleset m_ruleset = PATHFINDER_1E_DND_3_5E;
	bool m_rollAutomatically = true;

private:

	void
	readSettings();
};
