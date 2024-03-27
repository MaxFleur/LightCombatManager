#pragma once

#include "RuleSettings.hpp"
#include "CharacterHandler.hpp"
#include "CharDirSettings.hpp"
#include "BaseFileHandler.hpp"

#include <QJsonObject>

// This class handles the saving and opening of csv table data
class CharFileHandler: public BaseFileHandler {
public:
    // Write the character data to a file
    [[nodiscard]] bool
    writeToFile(const CharacterHandler::Character& character,
                         const QString&                     fileName) const;
    //remove a saved character file
    [[nodiscard]] bool
    removeCharacter(const QString& fileName);

    // Open a saved table
    [[nodiscard]] int
    getStatus(const QString& fileName) override;



    [[nodiscard]] QJsonObject&
    getData() override
    {
        return m_characterFile;
    }

    CharDirSettings
    getDirSettings()
    {
        return m_dirSettings;
    }

private:
    // Checks if a loaded lcm file is in the right format
    [[nodiscard]] bool
    checkFileFormat() const override;

private:
    QJsonObject m_characterFile;
    CharDirSettings m_dirSettings;
};