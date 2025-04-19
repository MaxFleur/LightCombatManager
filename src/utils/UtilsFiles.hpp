#pragma once

#include <QString>

#include <optional>

// Utils for file handling
namespace Utils::Files
{
// Remove a file
[[nodiscard]] bool
removeFile(const QString& fileName);

// Find a json object inside a directory by a certain name
std::optional<QString>
findObject(const QString& directory,
           const QString& fileEnding,
           const QString& objectName);
}
