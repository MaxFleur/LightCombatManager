#pragma once

#include "BaseSettings.hpp"

// Store the main settings affecting the entire application
class MainSettings : public BaseSettings {
public:

	MainSettings();

	enum Ruleset {
		PATHFINDER_1E_DND_3_5E	= 0,
		PATHFINDER_2E		= 1,
		DND_5E			= 2
	};

	void
	write(int	newRuleset,
	      bool	newRollAutomatical);

public:
	Ruleset ruleset;
	bool rollAutomatical;

private:

	void
	read() override;
};
