#include "../../../include/GUI/Table/TableWidget.hpp"

// Create the welcome widget at instantiation
TableWidget::TableWidget(CharacterHandlerRef charHandler, bool isDataStored, QString data, QWidget *parent)
	: m_char(charHandler), m_isDataStored(isDataStored), m_data(data)
{
	// Create the table
	createTableSkeleton();
	// Now set the table data
	setTableData();
	// Create the widget for the buttons
	createLowerWidget();
	// Then set the height this widget will be displayed with
	setHeight();
	// Connect drag and drop function, enabled each time a row is dragged
	connect(tableWidget, SIGNAL(cellEntered(int,int)), this, SLOT(dragAndDrop(int,int)));
	connect(tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(rowSelected()));
}


// This function creates the table of this widget
void
TableWidget::createTableSkeleton()
{
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
	// Set spinboxes for the hp column
	delegate = new SpinBoxDelegate(this);
	tableWidget->setItemDelegateForColumn(1, delegate);

	// Allocate the main layout, add the table widget to this
	tableLayout = new QVBoxLayout(this);
	tableLayout->addWidget(tableWidget);
}


// Set the data inside the table
void
TableWidget::setTableData()
{
	if (m_isDataStored) {
		// If the data has been stored, take it and split it into rows
		QStringList rowOfData = m_data.split("\n");
		QStringList rowData;

		// Set the table row count
		// @note For some reason, the splitting of the data in line 53 creates one empty, obsolete line
		// To ignore this line, decrement the row count and iteration number
		// The second line is the header and is also ignored, so we set decrement again and are at -2
		tableWidget->setRowCount(rowOfData.size() - 2);
		// Iterate over the row
		// // We start with 1 to ignore the stored header
		for (int x = 1; x < rowOfData.size() - 1; x++) {
			// Split into the different column data
			rowData = rowOfData.at(x).split(";");
			// Create the widget items for the table
			for (int y = 0; y < 4; y++) {
				tableWidget->setItem(x - 1, y, new QTableWidgetItem(rowData[y]));
			}
			// If we are at the first row (which contains information about round counter and the
			// player on the move), we get this data
			if (x == 1) {
				rowEntered = rowData[4].toInt();
				// Decrement before the incrementRoundCounter function is called
				roundCounter = rowData[5].toInt();
			}
		}
	} else {
		// If no data is provided via csv, set the data according to the vector of chars
		// Set the number of rows to characters created
		tableWidget->setRowCount(m_char->getCharacters().size());
		// Store the created character data in the table
		for (int i = 0; i < m_char->getCharacters().size(); i++) {
			// Store the name
			tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(m_char->getCharacters().at(i)->name)));
			// Same for hp
			tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(m_char->getCharacters().at(i)->hp)));
			// Check if the bool is true or false, return yes or no
			if (m_char->getCharacters().at(i)->isNPC) {
				tableWidget->setItem(i, 2, new QTableWidgetItem("X"));
			} else {
				tableWidget->setItem(i, 2, new QTableWidgetItem(" "));
			}
			// Store additional information
			tableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(m_char->getCharacters().at(i)->additionalInf)));
		}
		// Select the first row
		rowEntered = 0;
		// Then set the round counter to 0 before incrementing
		roundCounter = 1;
	}
	// Set the coluḿns containing the isNPC values as not visible
	for (int i = 0; i < tableWidget->rowCount(); i++) {
		tableWidget->item(i, 2)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
}


// Create the lower widget, including the button
void
TableWidget::createLowerWidget()
{
	// Allocate
	lowerLayout = new QHBoxLayout();
	// Create the exit button
	exitButton = new QPushButton("Exit");
	// Tooltip
	exitButton->setToolTip("Exit the combat. This cannot be undone.");
	// We start with the first round, so set the counter to 1
	roundCounterLabel = new QLabel;
	setEnterPlayerLabelData();
	// Set the player displaying label
	enterPlayerLabel = new QLabel;

	// Create a spacer widget to move the button to the right side
	QWidget *spacer = new QWidget();

	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	// Add the elements to the label
	lowerLayout->addWidget(roundCounterLabel);
	lowerLayout->addSpacing(30);
	lowerLayout->addWidget(enterPlayerLabel);
	lowerLayout->addWidget(spacer);
	lowerLayout->addWidget(exitButton);
	tableLayout->addLayout(lowerLayout);

	// Create the identifiers for the rows
	for (int i = 0; i < tableWidget->rowCount(); i++) {
		identifiers.push_back(i);
	}
	selectEnteredRow();
	setCurrentPlayer();
}


void
TableWidget::setHeight()
{
	for (int i = 0; i < tableWidget->rowCount(); i++) {
		height += tableWidget->rowHeight(i);
	}
	height += 120;
}


// Passes the name of the player on the move to the player enter label
void
TableWidget::setEnterPlayerLabelData()
{
	roundCounterLabel->setText("Round " + QString::number(roundCounter));
}


// Passes the current player's name to the label
void
TableWidget::setCurrentPlayer()
{
	enterPlayerLabel->setText("Current: " + tableWidget->item(rowEntered, 0)->text());
}


// This function enables drag and drop of table rows
// Which works by switching the values of a row with it's upper or lower "neighbor"
// This is done until the row is at the desired position
void
TableWidget::dragAndDrop(int row, int column)
{
	// New row index
	int newRow;

	// Depending on the current index, set the newRow value
	if (tableWidget->currentIndex().row() < row) {
		newRow = row - 1;
		// Swap the identifiers so they match correctly with their row
		std::iter_swap(identifiers.begin() + newRow, identifiers.begin() + row);
	} else if (tableWidget->currentIndex().row() > row) {
		newRow = row + 1;
		std::iter_swap(identifiers.begin() + newRow, identifiers.begin() + row);
	}
	// Return if they are equal
	else {
		return;
	}
	// The Widget Items containing the row data (surce and destination)
	QList<QTableWidgetItem *> rowItemSrc, rowItemDest;

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
	for (int i = 0; i < identifiers.size(); i++) {
		// Search for the row containing the identifier
		if (identifiers.at(i) == rowIdentifier) {
			// Set row enter to this
			rowEntered = i;
			break;
		}
	}
}


// Remove a row/character of the table
void
TableWidget::removeRow()
{
	// if only one character remains, remove this character
	// @note This part can change later, because the slot for selected rows is not called if only one row remains
	if (tableWidget->rowCount() == 1) {
		tableWidget->removeRow(0);
		return;
	}
	// If a row has been selected, remove this row
	if (isRowSelected) {
		// If the deleted row is before the current entered row, move rowEnter one up
		if (tableWidget->currentIndex().row() < rowEntered) {
			rowEntered--;
		}
		// If the deleted row was the last one in the table, select to the first row
		if (tableWidget->currentIndex().row() == tableWidget->rowCount() - 1) {
			rowEntered = 0;
		}
		// Now remove the row
		tableWidget->removeRow(tableWidget->currentIndex().row());
		// Then reset
		isRowSelected = false;
		// Reste the current entered player and highlight again
		setCurrentPlayer();
		selectEnteredRow();
		return;
	}
	// If no row has been clicked, throw an error message
	QMessageBox::StandardButton reply = QMessageBox::warning(this,
								 "Could not remove character!",
								 "Please select a character with the Mouse Key before deleting!");

	return;
}


// Check if a row is selected
void
TableWidget::rowSelected()
{
	// Set to true if no row has been selected
	if (!isRowSelected) {
		isRowSelected = true;
	}
}


// Select a row with the Enter key
void
TableWidget::selectEnteredRow()
{
	// First, clear all selected rows (not their data, just the selection)
	tableWidget->selectionModel()->clearSelection();
	// Now select the row that was selected using the Enter key
	tableWidget->selectRow(rowEntered);
}


// This function calls the row remove function if the delete key is pressed
void
TableWidget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete) {
		removeRow();
	}
	// Iterate over the rows with the Enter key
	if (event->key() == Qt::Key_Return) {
		// If the current selected row is the last one, reset to the first one
		if (rowEntered == tableWidget->rowCount() - 1) {
			rowEntered = 0;
			// Now the next row begins, so increment the counter
			roundCounter++;
			setEnterPlayerLabelData();
		} else {
			rowEntered++;
		}
		// The identifier gets the identifying value at the entered row
		rowIdentifier = identifiers.at(rowEntered);
		// Now select the row with Enter
		selectEnteredRow();
		// Show the current player
		setCurrentPlayer();
	}
}
