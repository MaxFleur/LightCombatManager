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

    // Apply the filename to recent saved directories
    if (std::find(std::begin(recentDirs), std::end(recentDirs), fileName) != recentDirs.end()) {
        return;
    }
    // Place in front
    std::shift_right(recentDirs.begin(), recentDirs.end(), 1);
    recentDirs[0] = fileName;
    // Resave recent dir values
    for (std::array<QString, 5>::size_type i = 0; i < recentDirs.size(); i++) {
        if (recentDirs.at(i).isEmpty()) {
            break;
        }

        settings.setValue("recent_dir_" + QString::number(i), recentDirs.at(i));
    }
}


void
DirSettings::read()
{
    QSettings settings;
    for (std::array<QString, 5>::size_type i = 0; i < recentDirs.size(); i++) {
        const auto& recentKey = "recent_dir_" + QString::number(i);
        if (!settings.value(recentKey).isValid()) {
            break;
        }

        if (std::filesystem::exists(settings.value(recentKey).toString().toStdString())
            && std::filesystem::path(settings.value(recentKey).toString().toStdString()).extension().string() == ".lcm") {
            recentDirs[i] = settings.value("recent_dir_" + QString::number(i)).toString();
        } else {
            // Might have turned invalid in the meantime
            settings.remove("recent_dir_" + QString::number(i));
        }
    }

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
