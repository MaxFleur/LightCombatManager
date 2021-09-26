#pragma once

#include <QtWidgets>
#include <memory>

// This class handles the saving and reopening of QTableWidgets as csv data
class FileHandler {
public:
// Constructor
FileHandler();
// Save a table of characters
void saveTable(QTableWidget *tableWidget, QString filename, int rowEntered, int roundCounter);
// Reopen a saved table
int getCSVData(QString filename);

QString getData()
{
	return m_data;
}

private:
// Data used to fill the table
QString m_data;
// Checks if a table is in the right format
bool checkTableFormat(QString data);
};
using FileHandlerRef = std::shared_ptr<FileHandler>;
