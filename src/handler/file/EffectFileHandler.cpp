#include "EffectFileHandler.hpp"

#include <QDir>
#include <QFile>
#include <QJsonDocument>

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

    // Write to file
    const auto byteArray = QJsonDocument(effectObject).toJson();
    QFile fileOut(m_directoryString + "/" + name + ".effect");
    fileOut.open(QIODevice::WriteOnly);
    return fileOut.write(byteArray);
}


bool
EffectFileHandler::removeEffect(const QString& effectName)
{
    QFile fileOut(m_directoryString + effectName);
    if (!fileOut.exists()) {
        return false;
    }
    return fileOut.remove();
};


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
