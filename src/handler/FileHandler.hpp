#pragma once

#include <memory>

#include <QString>

#include "RuleSettings.hpp"

class QTableWidget;

// This class handles the saving and reopening of tables as csv data
class FileHandler {
public:

	// Save a table of characters
	[[nodiscard]] bool
	saveTable(QTableWidget *		tableWidget,
		  const QString&		filename,
		  unsigned int			rowEntered,
		  unsigned int			roundCounter,
		  const RuleSettings::Ruleset&	ruleset,
		  bool				rollAutomatically) const;

	// Reopen a saved table
	[[nodiscard]] int
	getCSVData(const QString& filename);

	[[nodiscard]] QString
	getData() const
	{
		return m_data;
	}

private:
	// Data used to fill the table
	QString m_data;

	// Checks if a table is in the right format
	bool
	checkTableFormat(const QString& data) const;
};
using FileHandlerRef = std::shared_ptr<FileHandler>;
