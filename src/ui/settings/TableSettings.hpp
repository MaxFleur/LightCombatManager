#pragma once

// Store data used for the table
class TableSettings {
public:

	TableSettings();

	void
	writeIsIniShown(bool isIniShown);

	void
	writeIsModifierShown(bool isModifierShown);

public:
	bool m_isIniShown = false;
	bool m_isModifierShown = false;

private:

	void
	read();
};
