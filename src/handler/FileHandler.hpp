#pragma once

#include "RuleSettings.hpp"

#include <QJsonObject>

// Handles saving and opening of lcm table data
class FileHandler {
public:
    // Write the table to an lcm file
    [[nodiscard]] bool
    writeTableToFile(const QVector<QVector<QVariant> >& tableData,
                     const QString&                     fileName,
                     unsigned int                       rowEntered,
                     unsigned int                       roundCounter,
                     const RuleSettings::Ruleset&       ruleset,
                     bool                               rollAutomatically) const;

    // Open a saved table
    [[nodiscard]] int
    getLCMStatus(const QString& fileName);

    [[nodiscard]] QJsonObject&
    getData()
    {
        return m_lcmFile;
    }

private:
    // Checks if a loaded lcm file is in the right format
    bool
    checkLCMFormat() const;

private:
    // Object storing file content
    QJsonObject m_lcmFile;
};
