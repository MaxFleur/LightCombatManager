#include "BaseFileHandler.hpp"

#include <QFile>
#include <QJsonDocument>

int
BaseFileHandler::getStatus(const QString& fileName)
{
    // Try to open
    QFile fileIin(fileName);
    if (!fileIin.open(QIODevice::ReadOnly)) {
        // Read failed
        return 2;
    }

    auto byteArray = fileIin.readAll();
    const auto document = QJsonDocument::fromJson(byteArray);
    m_fileData = document.object();
    // Correct or false format
    return !checkFileFormat();
}
