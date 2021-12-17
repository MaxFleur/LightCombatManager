#include "FileHandler.hpp"

#include <QCheckBox>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextStream>

#include "../utils/Utils.hpp"

// Stores a table as a csv
bool
FileHandler::saveTable(QTableWidget *tableWidget, QString filename, int rowEntered, int roundCounter)
{
	// Create a file
	QFile file(filename);

	// Check if device is open for writing
	if (file.open(QFile::WriteOnly | QFile::Truncate)) {
		QTextStream data(&file);
		data.setCodec("UTF-8");
		data.setGenerateByteOrderMark(false);

		QStringList strList;

		// Store the header of the table, used for checking the correct table format if the table is reloaded
		strList << "Name" << "Initiative" << "INI modifier" << "HP" << "Is Enemy" << "Additional information";
		data << strList.join(";") + "\n";

		for (int i = 0; i < tableWidget->rowCount(); ++i) {
			// Clear the list at the beginning of every row iteration
			strList.clear();
			// Ignore the last row, identifiers are not needed
			for (int j = 0; j < tableWidget->columnCount() - 1; ++j) {
				// Get the checkbox value
				if (j == 4) {
					auto * const checkBox = (QCheckBox *) tableWidget->cellWidget(i, j);
					strList << QVariant(checkBox->isChecked()).toString();
				} else {
					const auto *item = tableWidget->item(i, j);
					if (!item || item->text().isEmpty()) {
						tableWidget->setItem(i, j, new QTableWidgetItem(""));
					}
					strList << tableWidget->item(i, j)->text();
				}
			}
			if (i == 0) {
				strList << QString::number(rowEntered) << QString::number(roundCounter);
			}
			// The "\n" guarantees that the rows are set correctly in the csv table
			data << strList.join(";") + "\n";
		}
		file.close();
		return true;
	}
	return false;
}


// Open an existing csv table and stream it's data
int
FileHandler::getCSVData(QString filename)
{
	QFile importedCSV(filename);

	// If the data can be read, import
	if (importedCSV.open(QFile::ReadOnly)) {
		QTextStream in(&importedCSV);
		in.setCodec("UTF-8");
		in.setGenerateByteOrderMark(false);
		m_data = QString();

		// Import file line by line
		while (!in.atEnd()) {
			m_data.append(in.readLine() + "\n");
		}
		importedCSV.close();

		if (checkTableFormat(m_data)) {
			// Successfully checked table
			return 0;
		}
		// Table in false format
		return 1;
	}
	// Unreadable data
	return 2;
}


// Checks if a table is in the correct format before using
bool
FileHandler::checkTableFormat(QString data)
{
	// Get the stored table row data information
	auto const rowDataHeader = data.split("\n").at(0).split(";");
	auto const rowDataFirstRow = data.split("\n").at(1).split(";");

	// Test if the table has the correct header columns
	if (rowDataHeader.size() == 6
	    && rowDataHeader[0] == "Name"
	    && rowDataHeader[1] == "Initiative"
	    && rowDataHeader[2] == "INI modifier"
	    && rowDataHeader[3] == "HP"
	    && rowDataHeader[4] == "Is Enemy"
	    && rowDataHeader[5] == "Additional information"

	    // The second row is checked -> if the 7th entry contains the player on the move and
	    // the 8th the round counter, continue
	    && rowDataFirstRow.size() == 8) {
		return true;
	}
	return false;
}
