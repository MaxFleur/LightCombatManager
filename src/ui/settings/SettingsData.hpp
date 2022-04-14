#pragma once


// Dialog used to add certain status effects to characters
class SettingsData {
public:
	enum Ruleset {
		PATHFINDER_1E	= 0,
		PATHFINDER_2E	= 1,
		DND_3_5E	= 2,
		DND_5E		= 3
	};

public:
	Ruleset ruleset = PATHFINDER_1E;
    bool rollAutomatically = true;
};
