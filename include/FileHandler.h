#pragma once

#include <QtWidgets>

// This class handles the saving and reopening of QTableWidgets as csv data
class FileHandler : public QWidget {
    Q_OBJECT
    
public:
    // Constructor
    FileHandler(QWidget *parent = 0);
    // Save a table of characters
    void saveTable(QTableWidget *tableWidget);
    // Reopen a saved table
    QTableWidget* openTable();
    
};
