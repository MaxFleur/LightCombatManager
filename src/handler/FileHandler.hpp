#pragma once

#include <memory>

#include <QString>

class QTableWidget;

// This class handles the saving and reopening of tables as csv data
class FileHandler {
public:

	// Save a table of characters
	[[nodiscard]] bool
	saveTable(QTableWidget *tableWidget,
		  QString	filename,
		  int		rowEntered,
		  int		roundCounter);

	// Reopen a saved table
	[[nodiscard]] int
	getCSVData(QString filename);

	[[nodiscard]] QString
	getData()
	{
		return m_data;
	}

private:
	// Data used to fill the table
	QString m_data;

	// Checks if a table is in the right format
	bool
	checkTableFormat(QString data);
};
using FileHandlerRef = std::shared_ptr<FileHandler>;
