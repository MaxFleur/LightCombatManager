#include "DirSettings.hpp"

#include <QDir>
#include <QSettings>

DirSettings::DirSettings()
{
    handleSubDir();
    read();
}


void
DirSettings::write(const QString& fileName, bool setSaveDir)
{
    QSettings settings;
    writeParameter(settings, fileName, openDir, "dir_open");

    if (setSaveDir) {
        writeParameter(settings, fileName, saveDir, "dir_save");
        saveDir = fileName;
    }
}


void
DirSettings::read()
{
    QSettings settings;
    saveDir = settings.value("dir_save").isValid() ? settings.value("dir_save").toString() : QString();
    openDir = settings.value("dir_open").isValid() ? settings.value("dir_open").toString() : QString();
}


void
DirSettings::handleSubDir()
{
    // Create a subdir to save the tables into
    QDir dir(QDir::currentPath());
    // Write into settings so this subdir is used as standard path for saving tables
    if (dir.mkdir("tables")) {
        const auto tableSubDir = QDir::currentPath() + "/tables";
        // Save and open get the same directory, user might change it later
        saveDir = tableSubDir;
        openDir = tableSubDir;

        write(tableSubDir, true);
        return;
    }
}
