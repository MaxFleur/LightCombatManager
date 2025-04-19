#pragma once

#include "BaseSettings.hpp"

#include <QString>

#include <array>

// Store data used for handling the opening and saving directories
class DirSettings : public BaseSettings {
public:
    DirSettings();

    void
    write(const QString& fileName,
          bool           setSaveDir = false);

public:
    std::array<QString, 5> recentDirs;

    QString openDir;
    QString saveDir;

private:
    void
    read() override;

    void
    handleSubDir();
};
