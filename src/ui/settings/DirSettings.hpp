#pragma once

#include <QString>

#include "BaseSettings.hpp"

// Store data used for handling the opening and saving directories
class DirSettings : public BaseSettings {
public:
    DirSettings();

    void
    write(const QString& fileName,
          bool           setSaveDir = false);

public:
    QString openDir;
    QString saveDir;

private:
    void
    read() override;

    void
    handleSubDir();
};
