#include "TableSettings.hpp"

#include <QSettings>

TableSettings::TableSettings()
{
	read();
}


void
TableSettings::read()
{
	QSettings settings;

	settings.beginGroup("TableSettings");
	iniShown = settings.value("INI").isValid() ? settings.value("INI").toBool() : true;
	modifierShown = settings.value("Modifier").isValid() ? settings.value("Modifier").toBool() : true;
	settings.endGroup();
}


void
TableSettings::writeIniShown(bool newIniShown)
{
	QSettings settings;

	settings.beginGroup("TableSettings");
	iniShown = newIniShown;
	settings.setValue("INI", iniShown);
	settings.endGroup();
}


void
TableSettings::writeModifierShown(bool newModShown)
{
	QSettings settings;

	settings.beginGroup("TableSettings");
	modifierShown = newModShown;
	settings.setValue("Modifier", modifierShown);
	settings.endGroup();
}
