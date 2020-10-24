#pragma once

#include <QtWidgets>
#include "../CharacterHandler.h"
#include "CustomTable.h"
    
// This class handles the creation of the table widget
class TableWidget : public QWidget {
    Q_OBJECT
    
public:
    TableWidget(CharacterHandlerRef charHandler, bool isDataStored, QString data = "", QWidget *parent = 0);

    QPushButton* getExitButton() { return exitButton; }
    QTableWidget* getTableWidget() { return tableWidget; }
    
    // Return the height of this widget;
    int getHeight() { return height; }
        
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
    void createTableSkeleton();
    // Set the data inside the table
    void setTableData();
    // Create the lower widget
    void createLowerWidget();
    // Set the height of this widget
    void setHeight();
    // Increments the round counter
    void incrementRoundCounter();
    // Sets the text displaying the player selected with Enter
    void setCurrentPlayer();
    // Key event function
    void keyPressEvent(QKeyEvent *event);
    
    void selectEnteredRow();
    
    // Main table widget and header
    CustomTable     *tableWidget;
    QStringList     tableHeader;
    QHBoxLayout     *lowerLayout;
    // Button to exit the layout
    QPushButton     *exitButton;
    // Button to remove a row
    QPushButton     *removeButton;
    // The label displaying the current round number
    QLabel          *roundCounterLabel;
    // The label displaying the current player selected with Enter
    QLabel          *enterPlayerLabel;
    // The final layout that will be returned
    QVBoxLayout     *tableLayout;
    
    // Has a row been selected via clicking
    bool isRowSelected = false;
    // The height of this widget to be set
    int height = 0;
    
    // Check if the loaded data has been stored
    bool m_isDataStored;
    // Data to load the table
    QString m_data;
    
    // The entry number of the row selected with Enter
    int rowEntered;
    // Counts the current round
    int roundCounter;
    
    // This vector stores unique identifiers for each row
    std::vector<int> identifiers;
    // The row identifier to determine the correct row after drag and drop
    int rowIdentifier;
};
 
