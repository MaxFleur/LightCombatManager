#include "CharFileHandler.hpp"

#include "AdditionalInfoData.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

bool
CharFileHandler::writeToFile(const CharacterHandler::Character &character, const QString &fileName) const {
    QJsonObject characterObject;
    characterObject["name"] = character.name;
    characterObject["initiative"] = character.initiative;
    characterObject["modifier"] = character.modifier;
    characterObject["hp"] = character.hp;
    characterObject["is_enemy"] = character.isEnemy;
    characterObject["additional_info"] = character.additionalInfoData.mainInfoText;

    // Write to file
    auto byteArray = QJsonDocument(characterObject).toJson();
    QFile fileOut(m_dirSettings.saveDir + fileName);
    fileOut.open(QIODevice::WriteOnly);
    return fileOut.write(byteArray) != -1;
}
bool
CharFileHandler::removeCharacter(const QString& fileName){
    QFile fileOut(m_dirSettings.saveDir  + fileName);
    if (!fileOut.exists()) {
        return false;
    }
    return fileOut.remove();
};

int
CharFileHandler::getStatus(const QString& fileName)
{
    // Try to open
    QFile fileIin(m_dirSettings.openDir + fileName);
    if (!fileIin.open(QIODevice::ReadOnly)) {
        // Read failed
        return 2;
    }

    auto byteArray = fileIin.readAll();
    const auto document = QJsonDocument::fromJson(byteArray);
    m_characterFile = document.object();
    // Correct or false format
    return !checkFileFormat();
}
bool
CharFileHandler::checkFileFormat() const
{
    auto checker = !m_characterFile.empty() && m_characterFile.contains("name") && m_characterFile.contains("initiative") &&
                   m_characterFile.contains("modifier") && m_characterFile.contains("hp") &&
                   m_characterFile.contains("is_enemy") && m_characterFile.contains("additional_info");
    return checker;
}