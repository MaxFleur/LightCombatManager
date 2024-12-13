#pragma once

#include "BaseFileHandler.hpp"
#include "CharacterHandler.hpp"

// This class handles the saving and opening of effect templates
class EffectFileHandler : public BaseFileHandler {
public:
    EffectFileHandler();

    // Write effect
    [[nodiscard]] bool
    writeToFile(const QString& name) const;

    // Remove effect
    [[nodiscard]] bool
    removeEffect(const QString& effectName);

    // Open a saved table
    [[nodiscard]] int
    getStatus(const QString& fileName) override;

    const QString&
    getDirectoryString()
    {
        return m_directoryString;
    }

private:
    // Checks if a loaded lcm file is in the right format
    [[nodiscard]] bool
    checkFileFormat() const override;

private:
    QString m_directoryString;
};
