#pragma once

#include <QtWidgets>
#include <memory>

// This class handles the saving and reopening of QTableWidgets as csv data
class FileHandler {
    
public:
    // Constructor
    FileHandler();
    // Save a table of characters
    void saveTable(QTableWidget *tableWidget, QString filename);
    // Reopen a saved table
    bool getCSVData(QString filename);
    
    QString getData() { return m_data; }
    
private:
    QString m_data;
    bool checkTableFormat(QString data);
};
using FileHandlerRef = std::shared_ptr<FileHandler>;
