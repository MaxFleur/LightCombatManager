#pragma once

#include <QJsonObject>

class BaseFileHandler {
public:
    [[nodiscard]] virtual int
    getStatus(const QString& fileName);

    [[nodiscard]] QJsonObject&
    getData()
    {
        return m_fileData;
    }

protected:
    QJsonObject m_fileData;

private:
    [[nodiscard]] virtual bool
    checkFileFormat() const = 0;
};
