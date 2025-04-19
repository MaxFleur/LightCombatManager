#pragma once

#include <QJsonObject>

class BaseFileHandler {
public:
    virtual
    ~BaseFileHandler() = default;

    [[nodiscard]] virtual int
    getStatus(const QString& fileName);

    [[maybe_unused]] bool
    writeJsonObjectToFile(const QJsonObject& object,
                          const QString&     fileName) const;

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
