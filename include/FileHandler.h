#pragma once

#include <QtWidgets>

// This class handles the saving and reopening of QTableWidgets as csv data
class FileHandler {
    
public:
    // Constructor
    FileHandler();
    // Save a table of characters
    void saveTable(QTableWidget *tableWidget, QString filename);
    // Reopen a saved table
    bool getCSVData(QString filename);
    
    QString getData() { return data; }
    
private:
    QString data;
};
using FileHandlerRef = std::shared_ptr<FileHandler>;
