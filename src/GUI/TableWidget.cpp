#include "../../include/GUI/TableWidget.h"

// Create the welcome widget at instantiation
TableWidget::TableWidget(CharacterHandlerRef charHandler, QWidget *parent)
        : m_char(charHandler) {
    // Create the table
    createTable();
    // Then create the other widgets
    createLowerWidget();
    // Connect drag and drop function, enabled each time a row is dragged
    connect(tableWidget, SIGNAL(cellEntered(int,int)), this, SLOT(dragAndDrop(int,int)));
}

// This function creates the table of this widget
void TableWidget::createTable() {
    // Allocate the table widget
    tableWidget = new QTableWidget();
    // Set the number of rows to characters created
    tableWidget->setRowCount(m_char->getCharacters().size());
    // Four colums for name, hp, isNPC and additional information about a character. The last column is for the buttons
    tableWidget->setColumnCount(5);
    // Set the header infos, no info for the last column
    tableHeader << "Name" << "HP" << "Is NPC" << "Additional information" << "";
    tableWidget->setHorizontalHeaderLabels(tableHeader);
    // Store the created character data in the table
    for(int i = 0; i < m_char->getCharacters().size(); i++) {
        // Store the name
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(m_char->getCharacters().at(i)->name)));
        // Same for hp
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(m_char->getCharacters().at(i)->hp)));
        // Check if the bool is true or false, return yes or no
        if(m_char->getCharacters().at(i)->isNPC) {
            tableWidget->setItem(i, 2, new QTableWidgetItem("Yes"));
        } else {
            tableWidget->setItem(i, 2, new QTableWidgetItem("No"));
        }
        // Store additional information
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(m_char->getCharacters().at(i)->additionalInf)));
    }
    // Set the numbers on the headers to not visible
    tableWidget->verticalHeader()->setVisible(false);
    // Enable grid
    tableWidget->setShowGrid(true);
    // Entire rows are selected
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    // Set fixed sizes for the columns
    tableWidget->setColumnWidth(0, 100);
    tableWidget->setColumnWidth(1, 25);
    tableWidget->setColumnWidth(2, 50);
    tableWidget->setColumnWidth(3, 270);
    tableWidget->setColumnWidth(4, 180);
    
    // Allocate the main layout, add the table widget to this
    tableLayout = new QVBoxLayout(this);
    tableLayout->addWidget(tableWidget);
}

// Create the lower widget, including the button
void TableWidget::createLowerWidget() {
    // Allocate
    lowerLayout = new QHBoxLayout();
    // Create the new button
    exitButton = new QPushButton("Exit");
    // Tooltip
    exitButton->setToolTip("Exit the combat. This cannot be undone.");
    // Create a spacer widget to move the button to the right side
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // Add the spacer and button to the lower layout, then add the lower layout to the final layout
    lowerLayout->addWidget(spacer);
    lowerLayout->addWidget(exitButton);
    tableLayout->addLayout(lowerLayout);
}

// This function enables drag and drop of table rows
// Which works by switching the values of a row with it's upper or lower "neighbor"
// This is done until the row is at the desired position
void TableWidget::dragAndDrop(int row, int column) {
    // New row index
    int newRow;
    // Depending on the current index, set the newRow value
    if(tableWidget->currentIndex().row() < row) {
        newRow = row - 1; 
    }
    else if(tableWidget->currentIndex().row() > row) {
        newRow = row + 1; 
    }
    // Return if they are equal
    else return;
    // The Widget Items containing the row data (surce and destination)
    QList<QTableWidgetItem*> rowItemSrc, rowItemDest;
    // Take the data of the source and destination row
    for (int col = 0; col < tableWidget->columnCount(); ++col) {
        rowItemSrc << tableWidget->takeItem(row, col);
        rowItemDest << tableWidget->takeItem(newRow, col);
    }
    // Set them in reversed order
    for (int col = 0; col < tableWidget->columnCount(); ++col) {
        tableWidget->setItem(newRow, col, rowItemSrc.at(col));
        tableWidget->setItem(row, col, rowItemDest.at(col));
    }
}

TableWidget::~TableWidget() {
    // TODO: Delete the widget items
}
 
