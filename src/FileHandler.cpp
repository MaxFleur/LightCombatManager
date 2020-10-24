#include "../include/FileHandler.h" 

FileHandler::FileHandler() {
} 

// Stores a table with characters as a csv file
void FileHandler::saveTable(QTableWidget *tableWidget, QString filename) {
    // Create a file using the given filename
    QFile file(filename);
    // Check if device is open for writing
    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        // Create a text stream streaming the input data
        QTextStream data( &file );
        // Stringlist for the row data
        QStringList strList;
        
        // Store the header of the table, this is used for checking the correct table format if the table is reloaded
        strList << "Name" << "HP" << "Is NPC" << "Additional information";
        data << strList.join( ";" ) + "\n";
        
        // Iterate over the rows
        for(int i = 0; i < tableWidget->rowCount(); ++i ) {
            // Clear the list at the beginning of every row iteration
            strList.clear();
            // Iterate over every column for each row
            for(int j = 0; j < tableWidget->columnCount(); ++j ) {
                // Get the items of every cell of the table
                QTableWidgetItem* item = tableWidget->item(i, j);
                // Check if the cell is empty (for example, if no additional information was provided)
                if (!item || item->text().isEmpty()) {
                    // If this is the case, write a whitespace so the cell can be added correctly
                    tableWidget->setItem(i, j, new QTableWidgetItem(" "));
                }
                // For each row, every cell will be added to the string list
                strList << tableWidget->item(i, j)->text();
            }
            // Now put the list to the text stream 
            // The "\n" guarantees that the rows are set correctly in the csv table
            data << strList.join( ";" ) + "\n";
        }
        // After the operation, close the file descriptor
        file.close();
    }
}

// Open an existing csv table
bool FileHandler::getCSVData(QString filename) {
    // Create a file using the given path
    QFile importedCSV(filename);
    // If the data can be read, import the csv data to the QFile
    if (importedCSV.open(QFile::ReadOnly)) {
        data = importedCSV.readAll();
        // Then close and return succesful
        importedCSV.close();
        return true;
    }
    // If the device was not readable, return false
    return false;
}
