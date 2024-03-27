#include "CharDirSettings.hpp"

#include <QDir>
#include <QSettings>

CharDirSettings::CharDirSettings()
{
    handleSubDir();
    read();
}


void
CharDirSettings::write(const QString& fileName, bool setSaveDir)
{
    openDir = fileName;

    QSettings settings;
    settings.setValue("char_dir_open", fileName);

    if (setSaveDir) {
        settings.setValue("char_dir_save", fileName);
        saveDir = fileName;
    }
}


void
CharDirSettings::read()
{
    QSettings settings;
    //TODO: Bug, dir_save has to be unique key, duplicated in DirSettings
    saveDir = settings.value("char_dir_save").isValid() ? settings.value("char_dir_save").toString() : QString();
    openDir = settings.value("char_dir_open").isValid() ? settings.value("char_dir_open").toString() : QString();
}


void
CharDirSettings::handleSubDir()
{
    // Create a subdir to save the tables into
    QDir dir(QDir::currentPath());
    // Write into settings so this subdir is used as standard path for saving tables
    if (!dir.exists("characters")){
        // Exit gracefully if necessary directory cannot be created?
        if (!dir.mkdir("characters")) return;
    }
    const auto tableSubDir = QDir::currentPath() + "/characters/";
    // Save and open get the same directory, user might change it later
    saveDir = tableSubDir;
    openDir = tableSubDir;
    write(tableSubDir, true);
}
