#include "../include/FileHandler.hpp"

FileHandler::FileHandler()
{
}


// Stores a table with characters as a csv file
void FileHandler::saveTable(QTableWidget *tableWidget, QString filename, int rowEntered, int roundCounter)
{
	// Create a file using the given filename
	QFile file(filename);

	// Check if device is open for writing
	if (file.open(QFile::WriteOnly | QFile::Truncate)) {
		// Create a text stream streaming the input data
		QTextStream data(&file);
		// Stringlist for the row data
		QStringList strList;

		// Store the header of the table, this is used for checking the correct table format if the table is reloaded
		strList << "Name" << "HP" << "Is NPC" << "Additional information";
		data << strList.join(";") + "\n";

		// Iterate over the rows
		for (int i = 0; i < tableWidget->rowCount(); ++i) {
			// Clear the list at the beginning of every row iteration
			strList.clear();
			// Iterate over every column for each row
			for (int j = 0; j < tableWidget->columnCount(); ++j) {
				// Get the items of every cell of the table
				QTableWidgetItem *item = tableWidget->item(i, j);
				// Check if the cell is empty (for example, if no additional information was provided)
				if (!item || item->text().isEmpty()) {
					// If this is the case, write a whitespace so the cell can be added correctly
					tableWidget->setItem(i, j, new QTableWidgetItem(" "));
				}
				// For each row, every cell will be added to the string list
				strList << tableWidget->item(i, j)->text();
			}
			if (i == 0) {
				strList << QString::number(rowEntered) << QString::number(roundCounter);
			}
			// Now put the list to the text stream
			// The "\n" guarantees that the rows are set correctly in the csv table
			data << strList.join(";") + "\n";
		}
		// After the operation, close the file descriptor
		file.close();
	}
}


// Open an existing csv table and stream it's data to m_data
int FileHandler::getCSVData(QString filename)
{
	// Create a file using the given path
	QFile importedCSV(filename);

	// If the data can be read, import the csv data to the QFile
	if (importedCSV.open(QFile::ReadOnly)) {
		m_data = importedCSV.readAll();
		// Then close the file
		importedCSV.close();
		if (checkTableFormat(m_data)) {
			// Code for successfully checked table
			return 2;
		}
		// Code for table in false format
		return 1;
	}
	// Code for unreadable data
	return 0;
}


// Checks if a table is in the correct format before using it
bool FileHandler::checkTableFormat(QString data)
{
	// Get the stored table row data information
	QStringList rowDataHeader = data.split("\n").at(0).split(";");
	QStringList rowDataFirstRow = data.split("\n").at(1).split(";");

	// Test if the table has four columns (the header columns)
	if (rowDataHeader.size() == 4
	    // Now test if the four columns in the first row have the correct header strings in them
	    && rowDataHeader[0] == "Name"
	    && rowDataHeader[1] == "HP"
	    && rowDataHeader[2] == "Is NPC"
	    && rowDataHeader[3] == "Additional information"
	    // The first row is checked -> if it has six entries (5th containing player on the move and
	    // 6th the round counter), we can continue
	    && rowDataFirstRow.size() == 6) {
		// True, so return
		return true;
	}// Otherwise return false
	return false;
}
