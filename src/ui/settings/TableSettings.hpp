#pragma once

#include "BaseSettings.hpp"

// Store settings used for customizing the table
class TableSettings : public BaseSettings {
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
	read() override;
};
