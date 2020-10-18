#include "../../include/GUI/TableWidget.h"

// Create the welcome widget at instantiation
TableWidget::TableWidget(CharacterHandlerRef charHandler, bool isDataStored, QString data, QWidget *parent)
        : m_char(charHandler), m_isDataStored(isDataStored), m_data(data) {
    // Create the table
    createTableSkeleton();
    // Create the widget for the buttons
    createLowerWidget();
    // Now set the table data
    setTableData();
    // Then set the height this widget will be displayed with
    setHeight();
    // Connect drag and drop function, enabled each time a row is dragged
    connect(tableWidget, SIGNAL (cellEntered(int,int)), this, SLOT(dragAndDrop(int,int)));
    connect(removeButton, SIGNAL (clicked ()), this, SLOT(removeRow()));
    connect(tableWidget->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(rowSelected()));
}

// This function creates the table of this widget
void TableWidget::createTableSkeleton() {
    // Allocate the table widget
    tableWidget = new CustomTable();
    // Four colums for name, hp, isNPC and additional information about a character. The last column is for the buttons
    tableWidget->setColumnCount(4);
    // Set the header infos, no info for the last column
    tableHeader << "Name" << "HP" << "Is NPC" << "Additional information" << "";
    tableWidget->setHorizontalHeaderLabels(tableHeader);
    // Set the numbers on the headers to not visible
    tableWidget->verticalHeader()->setVisible(false);
    // Enable grid
    tableWidget->setShowGrid(true);
    // Entire rows are selected
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    // Set fixed sizes for the columns
    tableWidget->setColumnWidth(0, 120);
    tableWidget->setColumnWidth(1, 35);
    tableWidget->setColumnWidth(2, 60);
    tableWidget->setColumnWidth(3, 395);
    
    // Allocate the main layout, add the table widget to this
    tableLayout = new QVBoxLayout(this);
    tableLayout->addWidget(tableWidget);
}

// Set the data inside the table
void TableWidget::setTableData() {
    if(m_isDataStored) {
        // If the data has been stored, take it and split it into rows
        QStringList rowOfData = m_data.split("\n");
        QStringList rowData;
        
        // Set the table row count
        // @note For some reason, the splitting of the data in line 53 creates one empty, obsolete line
        // To ignore this line, decrement the row count and iteration number 
        tableWidget->setRowCount(rowOfData.size() - 1);
        // Iterate over the row
        for (int x = 0; x < rowOfData.size() - 1; x++) {
            // Split into the different column data
            rowData = rowOfData.at(x).split(";");
            // Create the widget items for the table
            for (int y = 0; y < rowData.size(); y++) {
                tableWidget->setItem(x, y, new QTableWidgetItem(rowData[y]));
            }
        }
    } else {
        // If no data is provided via csv, set the data according to the vector of chars
        // Set the number of rows to characters created
        tableWidget->setRowCount(m_char->getCharacters().size());
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
    }
    // Create the identifiers for the rows
    for(int i = 0; i < tableWidget->rowCount(); i++) {
        identifiers.push_back(i);
    }
    // Select the first row 
    rowEntered = 0;
    selectEnteredRow();
}

// Create the lower widget, including the button
void TableWidget::createLowerWidget() {
    // Allocate
    lowerLayout = new QHBoxLayout();
    // Create the exit button
    exitButton = new QPushButton("Exit");
    // Tooltip
    exitButton->setToolTip("Exit the combat. This cannot be undone.");
    // Same for the remove button
    removeButton = new QPushButton("Remove character");
    removeButton->setToolTip("Removes a character from the list.");
    // We start with the first round, so set the counter to 1
    roundCounterLabel = new QLabel;
    roundCounter = 0;
    incrementRoundCounter();
    
    // Create a spacer widget to move the button to the right side
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    // Add the elements to the label
    lowerLayout->addWidget(roundCounterLabel);
    lowerLayout->addSpacing(30);
    lowerLayout->addWidget(spacer);
    lowerLayout->addWidget(removeButton);
    lowerLayout->addWidget(exitButton);
    tableLayout->addLayout(lowerLayout);
}

void TableWidget::setHeight() {
    for(int i = 0; i < tableWidget->rowCount(); i++) {
        height += tableWidget->rowHeight(i);
    }
    height += 120;
}

// Increments the row counter every time navigating with Enter starts again at the beginning
void TableWidget::incrementRoundCounter() {
    // Increment
    roundCounter++;
    roundCounterLabel->setText("Round " + QString::number(roundCounter));
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
        // Swap the identifiers so they match correctly with their row
        std::iter_swap(identifiers.begin() + newRow, identifiers.begin() + row);
    }
    else if(tableWidget->currentIndex().row() > row) {
        newRow = row + 1; 
        std::iter_swap(identifiers.begin() + newRow, identifiers.begin() + row);
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
    // After the drag and drop, the correct entered row has to be highlighted
    for(int i = 0; i < identifiers.size(); i++) {
        // Search for the row containing the identifier
        if(identifiers.at(i) == rowIdentifier) {
            // Set row enter to this
            rowEntered = i;
            break;
        }
    }
}

// Remove a row/character of the table
void TableWidget::removeRow() {
    // if only one character remains, remove this character 
    // @note This part can change later, because the slot for selected rows is not called if only one row remains
    if(tableWidget->rowCount() == 1) {
        tableWidget->removeRow(0);
        return;
    }
    // If a row has been selected, remove the selected row
    if(isRowSelected) {
        tableWidget->removeRow(tableWidget->currentIndex().row());
        // Then reset
        isRowSelected = false;
    }
}

// Check if a row is selected
void TableWidget::rowSelected() {
    // Set to true if no row has been selected
    if(!isRowSelected){
        isRowSelected = true;
    }
}

// Select a row with the Enter key
void TableWidget::selectEnteredRow() {
    // First, clear all selected rows (not their data, just the selection)
    tableWidget->selectionModel()->clearSelection();
    // Now select the row that was selected using the Enter key
    tableWidget->selectRow(rowEntered);
}

// This function calls the row remove function if the delete key is pressed
void TableWidget::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Delete) {
        removeRow();
    }
    // Iterate over the rows with the Enter key
    if(event->key() == Qt::Key_Return) {
        // If the current selected row is the last one, reset to the first one
        if(rowEntered == tableWidget->rowCount() - 1) {
            rowEntered = 0;
            // Now the next row begins, so increment the counter
            incrementRoundCounter();
        } else {
            rowEntered++;
        }
        // The identifier gets the identifying value at the entered row
        rowIdentifier = identifiers.at(rowEntered);
        // Now select the row with Enter
        selectEnteredRow();
    }
}
