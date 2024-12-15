#pragma once

#include "BaseFileHandler.hpp"
#include "CharacterHandler.hpp"

// This class handles the saving and opening of character templates
class CharFileHandler : public BaseFileHandler {
public:
    CharFileHandler();

    // Write the character data to a file
    [[nodiscard]] bool
    writeToFile(const CharacterHandler::Character& character) const;

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
