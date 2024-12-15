#pragma once

#include <QString>

// Utils for file handling
namespace Utils::Files
{
// Remove a file
[[nodiscard]] bool
removeFile(const QString& fileName);
}
