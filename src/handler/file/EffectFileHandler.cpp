#include "EffectFileHandler.hpp"

#include <QDir>

EffectFileHandler::EffectFileHandler()
{
    // Create a subdir to save the tables into
    QDir dir(QDir::currentPath());
    if (!dir.exists(QDir::currentPath() + "/effects")) {
        dir.mkdir("effects");
    }
    m_directoryString = QDir::currentPath() + "/effects/";
}


bool
EffectFileHandler::writeToFile(const QString& name) const
{
    QJsonObject effectObject;
    effectObject["name"] = name;

    return BaseFileHandler::writeJsonObjectToFile(effectObject, m_directoryString + "/" + name + ".effect");
}


int
EffectFileHandler::getStatus(const QString& fileName)
{
    return BaseFileHandler::getStatus(m_directoryString + fileName);
}


bool
EffectFileHandler::checkFileFormat() const
{
    return !m_fileData.empty() && m_fileData.contains("name");
}
