#pragma once

#include <QtWidgets>
#include "../CharacterHandler.h"
#include <iostream>
    
// This class handles the table widget
class TableWidget : public QWidget {
    Q_OBJECT
    
public:
    TableWidget(CharacterHandlerRef charHandler, QWidget *parent = 0);
    ~TableWidget();
    
private slots:
    // Enable drag and drop in the widget
    void dragAndDrop(int row, int column);
    
private:
    CharacterHandlerRef m_char;
    // Create the table
    void createTable();
    void createLowerWidget();
    
    // Main table widget and header
    QTableWidget    *tableWidget;
    QStringList     tableHeader;
    QHBoxLayout     *lowerLayout;
    // Button to exit the layout
    QPushButton     *exitButton;
    // Button to remove a row
    QPushButton     *removeButton;
    
    // The final layout that will be returned
    QVBoxLayout     *tableLayout;
};
 
