#pragma once

#include <QtWidgets>
#include "../CharacterHandler.h"
#include <iostream>
    
// This class handles the table widget
class TableWidget : public QWidget {
    Q_OBJECT
    
public:
    TableWidget(CharacterHandlerRef charHandler, QWidget *parent = 0);
    
    QPushButton* getExitButton() { return exitButton; }
    QTableWidget* getTableWidget() { return tableWidget; }
    
private slots:
    // Enable drag and drop in the widget
    void dragAndDrop(int row, int column);
    // Remove a row out of te table
    void removeRow();
    // Function activated if a row is selected
    void rowSelected();
    
private:
    CharacterHandlerRef m_char;
    // Create the table
    void createTable();
    // Create the lower widget
    void createLowerWidget();
    // Key event function
    void keyPressEvent(QKeyEvent *event);
    
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
    // Has a row been selected (via clicking or using the arrow keys)
    bool isRowSelected = false;
};
 
