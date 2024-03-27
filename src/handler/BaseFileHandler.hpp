#pragma once

#include <QVariant>
#include <QJsonObject>
#include "BaseSettings.hpp"
#include "RuleSettings.hpp"
#include "CharacterHandler.hpp"

class BaseFileHandler {
public:
    [[nodiscard]] virtual int
    getStatus(const QString& fileName) = 0;

    [[nodiscard]] virtual QJsonObject&
    getData()
    {
        return m_fileData;
    }
private:
    [[nodiscard]] virtual bool
    checkFileFormat() const = 0;

    QJsonObject m_fileData;
};