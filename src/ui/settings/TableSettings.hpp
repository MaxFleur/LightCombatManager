#pragma once

// Store data used for the table
class TableSettings {
public:

	TableSettings();

	void
	writeIniShown(bool iniShown);

	void
	writeModifierShown(bool modShown);

public:
	bool iniShown = false;
	bool modifierShown = false;

private:

	void
	read();
};
