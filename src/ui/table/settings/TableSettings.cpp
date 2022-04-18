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
	// If the application is called for the first time, initiative and
	// modifier are shown per default
	m_isIniShown = settings.value("INI").isValid() ? settings.value("INI").toBool() : true;
	m_isModifierShown = settings.value("Modifier").isValid() ? settings.value("Modifier").toBool() : true;
	settings.endGroup();
}


void
TableSettings::writeIsIniShown(bool isIniShown)
{
	QSettings settings;

	settings.beginGroup("TableSettings");
	if (m_isIniShown != isIniShown) {
		m_isIniShown = isIniShown;
		settings.setValue("INI", m_isIniShown);
	}

	settings.endGroup();
}


void
TableSettings::writeIsModifierShown(bool isModifierShown)
{
	QSettings settings;

	settings.beginGroup("TableSettings");
	if (m_isModifierShown != isModifierShown) {
		m_isModifierShown = isModifierShown;
		settings.setValue("Modifier", m_isModifierShown);
	}

	settings.endGroup();
}
