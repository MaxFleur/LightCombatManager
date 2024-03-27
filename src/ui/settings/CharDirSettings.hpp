#pragma once

#include "BaseSettings.hpp"

#include <QString>

class CharDirSettings  : public BaseSettings {
public:
    CharDirSettings();

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