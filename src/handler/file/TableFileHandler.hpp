#pragma once

#include "BaseFileHandler.hpp"
#include "RuleSettings.hpp"

// This class handles the saving and opening of csv table data
class TableFileHandler : public BaseFileHandler {
public:
    // Write the table to an lcm file
    [[nodiscard]] bool
    writeToFile(const QVector<QVector<QVariant> >& tableData,
                const QString&                     fileName,
                unsigned int                       rowEntered,
                unsigned int                       roundCounter,
                const RuleSettings::Ruleset&       ruleset,
                bool                               rollAutomatically) const;

private:
    // Checks if a loaded lcm file is in the right format
    bool
    checkFileFormat() const override;
};
