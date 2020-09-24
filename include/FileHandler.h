#pragma once

#include <QtWidgets>

// This class handles the saving and reopening of QTableWidgets as csv data
class FileHandler {
    
public:
    // Constructor
    FileHandler();
    // Save a table of characters
    void saveTable(QTableWidget *tableWidget);
    // Reopen a saved table
    QTableWidget* openTable();
    
};
using FileHandlerRef = std::shared_ptr<FileHandler>; 
