#include "DirSettings.hpp"

#include <QDir>
#include <QSettings>

DirSettings::DirSettings()
{
	handleSubDir();
	read();
}


void
DirSettings::read()
{
	QSettings settings;
	saveDir = settings.value("dir_save").isValid() ? settings.value("dir_save").toString() : QString();
	openDir = settings.value("dir_open").isValid() ? settings.value("dir_open").toString() : QString();
}


void
DirSettings::write(const QString& fileName, bool setSaveDir)
{
	openDir = fileName;

	QSettings settings;
	settings.setValue("dir_open", fileName);
	// Only set the standard save dir at the first program start, after that it's saved
	if (setSaveDir) {
		settings.setValue("dir_save", fileName);
	}
}


void
DirSettings::handleSubDir()
{
	// Create a subdir to save the tables into
	QDir dir(QDir::currentPath());
	// Write into settings so this subdir is used as standard path for saving tables
	if (dir.mkdir("tables")) {
		const auto tableSubDir = QDir::currentPath() + "/tables";
		// Save and open get same directory, user might change it later
		saveDir = tableSubDir;
		openDir = tableSubDir;

		write(tableSubDir, true);
		return;
	}
}
