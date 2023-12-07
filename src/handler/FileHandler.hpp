#pragma once

#include "RuleSettings.hpp"

#include <QVariant>

#include <memory>

// This class handles the saving and opening of csv table data
class FileHandler {
public:
    // Save the characters
    [[nodiscard]] bool
    saveTable(const QVector<QVector<QVariant> >& tableData,
              const QString&                     filename,
              unsigned int                       rowEntered,
              unsigned int                       roundCounter,
              const RuleSettings::Ruleset&       ruleset,
              bool                               rollAutomatically) const;

    // Open a saved table
    [[nodiscard]] int
    getCSVStatus(const QString& filename);

    [[nodiscard]] QString
    getData() const
    {
        return m_data;
    }

private:
    // Checks if data is in the right format
    bool
    checkTableFormat(const QString& data) const;

private:
    // Data used to fill the table
    QString m_data;
};
using FileHandlerRef = std::shared_ptr<FileHandler>;
