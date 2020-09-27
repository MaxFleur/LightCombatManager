#pragma once

#include <QtWidgets>
#include "../CharacterHandler.h"
    
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
    // The height of this widget to be set
    int height = 0;
    
    bool m_isDataStored;
    QString m_data;
};
 
