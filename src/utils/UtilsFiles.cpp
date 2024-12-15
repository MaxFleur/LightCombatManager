#include "UtilsFiles.hpp"

#include <QFile>

namespace Utils::Files
{
bool
removeFile(const QString& fileName)
{
    QFile fileOut(fileName);
    if (!fileOut.exists()) {
        return false;
    }
    return fileOut.remove();
};
}
