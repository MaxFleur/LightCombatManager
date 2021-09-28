#include "../../../include/GUI/Table/TableWidget.hpp"

#include <QAbstractItemView>
#include <QFont>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSizePolicy>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>

// Create the welcome widget at instantiation
TableWidget::TableWidget(CharacterHandlerRef charHandler, bool isDataStored, QString data, QWidget *parent)
	: m_char(charHandler), m_isDataStored(isDataStored), m_data(data)
{
	mp_tableWidget = new CustomTable();
	mp_tableWidget->setColumnCount(4);

	QStringList tableHeader;
	tableHeader << "Name" << "HP" << "Is NPC" << "Additional information" << "";

	mp_tableWidget->setHorizontalHeaderLabels(tableHeader);
	mp_tableWidget->verticalHeader()->setVisible(false);
	mp_tableWidget->setShowGrid(true);
	mp_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	mp_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	mp_tableWidget->setColumnWidth(0, 120);
	mp_tableWidget->setColumnWidth(1, 35);
	mp_tableWidget->setColumnWidth(2, 60);
	mp_tableWidget->setColumnWidth(3, 395);

	// Spinbox for the hp column
	auto *delegate = new SpinBoxDelegate(this);
	mp_tableWidget->setItemDelegateForColumn(1, delegate);

	auto *tableLayout = new QVBoxLayout(this);
	tableLayout->addWidget(mp_tableWidget);
	setTableData();

	// Create the exit button
	m_exitButton = new QPushButton("Exit");
	m_exitButton->setToolTip("Exit the combat. This cannot be undone.");

	m_currentPlayerLabel = new QLabel;
	m_roundCounterLabel = new QLabel;
	setRoundCounterData();

	// Create a spacer widget to move the button to the right side
	auto *spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	// Lower layout
	auto *lowerLayout = new QHBoxLayout();
	lowerLayout->addWidget(m_roundCounterLabel);
	lowerLayout->addSpacing(30);
	lowerLayout->addWidget(m_currentPlayerLabel);
	lowerLayout->addWidget(spacer);
	lowerLayout->addWidget(m_exitButton);
	tableLayout->addLayout(lowerLayout);

	// Create the identifiers for the rows
	for (int i = 0; i < mp_tableWidget->rowCount(); i++) {
		m_identifiers.push_back(i);
	}

	m_defaultFont.setBold(false);
	m_boldFont.setBold(true);
	setRowAndPlayer();

	setHeight();

	connect(mp_tableWidget, &QTableWidget::cellEntered, this, &TableWidget::dragAndDrop);
	connect(mp_tableWidget, &QTableWidget::cellClicked, this, &TableWidget::rowSelected);
	connect(
		m_exitButton,
		&QPushButton::clicked,
		this,
		[this] () {
			emit exit();
		});
}


// Set the data inside the table
void
TableWidget::setTableData()
{
	if (m_isDataStored) {
		const auto rowOfData = m_data.split("\n");
		QStringList rowData;

		// @note For some reason, the splitting of the data creates one empty, obsolete line
		// To ignore this line, decrement the row count and iteration number
		// The second line is the header and is also ignored, so decrement again and be at -2
		mp_tableWidget->setRowCount(rowOfData.size() - 2);
		// Ignore stored header
		for (int x = 1; x < rowOfData.size() - 1; x++) {
			rowData = rowOfData.at(x).split(";");
			// Create the widget items for the table
			for (int y = 0; y < 4; y++) {
				mp_tableWidget->setItem(x - 1, y, new QTableWidgetItem(rowData[y]));
			}
			// If at the first row (which contains information about round counter and the
			// player on the move), get this data
			if (x == 1) {
				m_rowEntered = rowData[4].toInt();
				m_roundCounter = rowData[5].toInt();
			}
		}
	} else {
		// If no data is provided via csv, set the data according to the vector of chars
		mp_tableWidget->setRowCount(m_char->getCharacters().size());

		for (int i = 0; i < m_char->getCharacters().size(); i++) {
			// Store char stats
			mp_tableWidget->setItem(i, 0, new QTableWidgetItem(m_char->getCharacters().at(i)->name));
			mp_tableWidget->setItem(
				i,
				1,
				new QTableWidgetItem(QString::number(m_char->getCharacters().at(i)->hp)));
			if (m_char->getCharacters().at(i)->isNPC) {
				mp_tableWidget->setItem(i, 2, new QTableWidgetItem("X"));
			} else {
				mp_tableWidget->setItem(i, 2, new QTableWidgetItem(" "));
			}
			mp_tableWidget->setItem(
				i,
				3,
				new QTableWidgetItem(m_char->getCharacters().at(i)->additionalInf));
		}
		m_rowEntered = 0;
		m_roundCounter = 1;
	}
	// Set the coluḿns containing the isNPC values as not visible
	for (int i = 0; i < mp_tableWidget->rowCount(); i++) {
		mp_tableWidget->item(i, 2)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
}


void
TableWidget::setHeight()
{
	for (int i = 0; i < mp_tableWidget->rowCount(); i++) {
		m_height += mp_tableWidget->rowHeight(i);
	}
	m_height += 120;
}


// Passes the name of the player on the move to the player enter label
void
TableWidget::setRoundCounterData()
{
	m_roundCounterLabel->setText("Round " + QString::number(m_roundCounter));
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
	if (mp_tableWidget->currentIndex().row() < row) {
		newRow = row - 1;
		// Swap the identifiers so they match correctly with their row
		std::iter_swap(m_identifiers.begin() + newRow, m_identifiers.begin() + row);
	} else if (mp_tableWidget->currentIndex().row() > row) {
		newRow = row + 1;
		std::iter_swap(m_identifiers.begin() + newRow, m_identifiers.begin() + row);
	} else {
		return;
	}
	// The Widget Items containing the row data (source and destination)
	QList<QTableWidgetItem *> rowItemSrc, rowItemDest;

	// Take the data of the source and destination row
	for (int col = 0; col < mp_tableWidget->columnCount(); ++col) {
		rowItemSrc << mp_tableWidget->takeItem(row, col);
		rowItemDest << mp_tableWidget->takeItem(newRow, col);
	}
	// Set them in reversed order
	for (int col = 0; col < mp_tableWidget->columnCount(); ++col) {
		mp_tableWidget->setItem(newRow, col, rowItemSrc.at(col));
		mp_tableWidget->setItem(row, col, rowItemDest.at(col));
	}
	// After the drag and drop, the correct entered row has to be highlighted
	for (int i = 0; i < m_identifiers.size(); i++) {
		// Set row containing the identifier
		if (m_identifiers.at(i) == m_rowIdentifier) {
			m_rowEntered = i;
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
	if (mp_tableWidget->rowCount() == 1) {
		mp_tableWidget->removeRow(0);
		return;
	}
	// If a row has been selected, remove this row
	if (m_isRowSelected) {
		// If the deleted row is before the current entered row, move rowEnter one up
		if (mp_tableWidget->currentIndex().row() < m_rowEntered) {
			m_rowEntered--;
		}
		// If the deleted row was the last one in the table, select to the first row
		if (mp_tableWidget->currentIndex().row() == mp_tableWidget->rowCount() - 1) {
			m_rowEntered = 0;
		}
		mp_tableWidget->removeRow(mp_tableWidget->currentIndex().row());

		m_isRowSelected = false;
		setRowAndPlayer();

		return;
	}
	QMessageBox::StandardButton reply = QMessageBox::warning(
		this,
		"Could not remove character!",
		"Please select a character with the Mouse Key before deleting!");

	return;
}


// Check if a row is selected
void
TableWidget::rowSelected()
{
	if (!m_isRowSelected) {
		m_isRowSelected = true;
	}
}


void
TableWidget::setRowAndPlayer()
{
	// Select row entered with Return key
	mp_tableWidget->selectionModel()->clearSelection();

	// Clear rows containing bold text
	for (int i = 0; i < mp_tableWidget->rowCount(); i++) {
		if (mp_tableWidget->item(i, 0)->font().bold()) {
			for (int j = 0; j < mp_tableWidget->columnCount(); j++) {
				mp_tableWidget->item(i, j)->setFont(m_defaultFont);
			}
		}
	}
	mp_tableWidget->selectRow(m_rowEntered);
	// Highlight selected row with bold fonts
	for (int j = 0; j < mp_tableWidget->columnCount(); j++) {
		mp_tableWidget->item(m_rowEntered, j)->setFont(m_boldFont);
	}

	// Display current player
	m_currentPlayerLabel->setText("Current: " + mp_tableWidget->item(m_rowEntered, 0)->text());
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
		if (m_rowEntered == mp_tableWidget->rowCount() - 1) {
			m_rowEntered = 0;
			m_roundCounter++;
			setRoundCounterData();
		} else {
			m_rowEntered++;
		}
		m_rowIdentifier = m_identifiers.at(m_rowEntered);
		setRowAndPlayer();
	}
}
