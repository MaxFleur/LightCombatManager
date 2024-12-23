#include "UtilsFiles.hpp"

#include <QDirIterator>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace Utils::Files
{
bool
removeFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.exists()) {
        return false;
    }

    return file.remove();
};


std::optional<QString>
findObject(const QString& directory, const QString& fileEnding, const QString& objectName)
{
    QDirIterator it(directory, { fileEnding }, QDir::Files);
    while (it.hasNext()) {
        it.next();

        QFile file(it.filePath());
        file.open(QIODevice::ReadOnly);
        const auto jsonObject = QJsonDocument::fromJson(file.readAll()).object();
        file.close();

        if (jsonObject["name"] == objectName) {
            return it.filePath();
        }
    }

    return {};
}
}
