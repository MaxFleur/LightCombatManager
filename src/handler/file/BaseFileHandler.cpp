#include "BaseFileHandler.hpp"

#include <QFile>
#include <QJsonDocument>

int
BaseFileHandler::getStatus(const QString& fileName)
{
    // Try to open
    QFile fileIn(fileName);
    if (!fileIn.open(QIODevice::ReadOnly)) {
        // Read failed
        return 2;
    }

    auto byteArray = fileIn.readAll();
    const auto document = QJsonDocument::fromJson(byteArray);
    m_fileData = document.object();
    // Correct or false format
    return !checkFileFormat();
}


bool
BaseFileHandler::writeJsonObjectToFile(const QJsonObject& object, const QString& fileName) const
{
    const auto byteArray = QJsonDocument(object).toJson();
    QFile fileOut(fileName);
    fileOut.open(QIODevice::WriteOnly);
    return fileOut.write(byteArray) != -1;
}
