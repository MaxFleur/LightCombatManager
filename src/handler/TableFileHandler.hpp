#pragma once

#include "RuleSettings.hpp"
#include "CharacterHandler.hpp"
#include "CharDirSettings.hpp"
#include "BaseFileHandler.hpp"
#include "DirSettings.hpp"

#include <QJsonObject>

// This class handles the saving and opening of csv table data
class TableFileHandler: public BaseFileHandler {
public:
    // Write the table to an lcm file
    [[nodiscard]] bool
    writeToFile(const QVector<QVector<QVariant> >& tableData,
                const QString&                     fileName,
                unsigned int                       rowEntered,
                unsigned int                       roundCounter,
                const RuleSettings::Ruleset&       ruleset,
                bool                               rollAutomatically) const;

    // Open a saved table
    [[nodiscard]] int
    getStatus(const QString& fileName) override;


    [[nodiscard]] QJsonObject&
    getData() override
    {
        return m_lcmFile;
    }

private:
    // Checks if a loaded lcm file is in the right format
    bool
    checkFileFormat() const override;


private:
    // Object storing file content
    QJsonObject m_lcmFile;
};
