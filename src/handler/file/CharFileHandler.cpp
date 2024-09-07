#include "CharFileHandler.hpp"

#include <QDir>
#include <QFile>
#include <QJsonDocument>

CharFileHandler::CharFileHandler()
{
    // Create a subdir to save the tables into
    QDir dir(QDir::currentPath());
    if (!dir.exists(QDir::currentPath() + "/chars")) {
        dir.mkdir("chars");
    }
    m_directoryString = QDir::currentPath() + "/chars/";
}


bool
CharFileHandler::writeToFile(const CharacterHandler::Character &character) const
{
    QJsonObject characterObject;
    characterObject["name"] = character.name;
    characterObject["initiative"] = character.initiative;
    characterObject["modifier"] = character.modifier;
    characterObject["hp"] = character.hp;
    characterObject["is_enemy"] = character.isEnemy;
    characterObject["additional_info"] = character.additionalInfoData.mainInfoText;

    // Write to file
    const auto byteArray = QJsonDocument(characterObject).toJson();
    QFile fileOut(m_directoryString + "/" + character.name + ".char");
    fileOut.open(QIODevice::WriteOnly);
    return fileOut.write(byteArray);
}


bool
CharFileHandler::removeCharacter(const QString& fileName)
{
    QFile fileOut(m_directoryString + fileName);
    if (!fileOut.exists()) {
        return false;
    }
    return fileOut.remove();
};


int
CharFileHandler::getStatus(const QString& fileName)
{
    return BaseFileHandler::getStatus(m_directoryString + fileName);
}


bool
CharFileHandler::checkFileFormat() const
{
    auto checker = !m_fileData.empty() && m_fileData.contains("name") && m_fileData.contains("initiative") &&
                   m_fileData.contains("modifier") && m_fileData.contains("hp") &&
                   m_fileData.contains("is_enemy") && m_fileData.contains("additional_info");

    return checker;
}
