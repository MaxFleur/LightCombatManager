#include "../../../include/GUI/Table/TableWidget.hpp"

#include <QAbstractItemView>
#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QFont>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSizePolicy>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>

#include "../../../include/GUI/Table/Delegate.hpp"
#include "../../../include/GUI/Table/StatusEffectDialog.hpp"

TableWidget::TableWidget(CharacterHandlerRef charHandler, bool isDataStored, QString data, QWidget *parent)
	: m_char(charHandler), m_isDataStored(isDataStored), m_data(data)
{
	readSettings();

	m_tableWidget = new CustomTable();
	m_tableWidget->setColumnCount(6);

	QStringList tableHeader;
	tableHeader << "Name" << "INI" << "Mod" << "HP" << "Is NPC" << "Additional information" << "";

	m_tableWidget->setHorizontalHeaderLabels(tableHeader);
	m_tableWidget->verticalHeader()->setVisible(false);
	m_tableWidget->setShowGrid(true);
	m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	m_tableWidget->setColumnWidth(COL_NAME, WIDTH_NAME);
	m_tableWidget->setColumnWidth(COL_INI, WIDTH_INI);
	m_tableWidget->setColumnWidth(COL_MODIFIER, WIDTH_MODIFIER);
	m_tableWidget->setColumnWidth(COL_HP, WIDTH_HP);
	m_tableWidget->setColumnWidth(COL_NPC, WIDTH_NPC);
	setInfoColumnWidth();

	// Spinbox for the hp column
	auto *const delegate = new SpinBoxDelegate(this);
	m_tableWidget->setItemDelegateForColumn(COL_HP, delegate);

	auto *const tableLayout = new QVBoxLayout(this);
	tableLayout->addWidget(m_tableWidget);
	setTableData();

	m_exitButton = new QPushButton("Return to Main Window");
	m_addCharButton = new QPushButton("Add new Character");
	m_addCharButton->setToolTip("Add a new character to this combat.");

	m_currentPlayerLabel = new QLabel;
	m_roundCounterLabel = new QLabel;
	setRoundCounterData();

	// Create a spacer widget to move the buttons to the right side
	auto *const spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	// Lower layout
	auto *const lowerLayout = new QHBoxLayout();
	lowerLayout->addWidget(m_roundCounterLabel);
	lowerLayout->addSpacing(SPACING);
	lowerLayout->addWidget(m_currentPlayerLabel);
	lowerLayout->addWidget(spacer);
	lowerLayout->addWidget(m_addCharButton);
	lowerLayout->addWidget(m_exitButton);
	tableLayout->addLayout(lowerLayout);

	// Create the identifiers for the rows
	for (int i = 0; i < m_tableWidget->rowCount(); i++) {
		m_identifiers.push_back(i);
	}

	m_defaultFont.setBold(false);
	m_boldFont.setBold(true);
	setRowAndPlayer();

	setHeight();

	connect(m_tableWidget, &QTableWidget::cellEntered, this, &TableWidget::dragAndDrop);
	connect(m_tableWidget, &QTableWidget::cellClicked, this, &TableWidget::rowSelected);
	connect(
		m_exitButton,
		&QPushButton::clicked,
		this,
		[this] () {
			emit exit();
		});
	connect(
		m_addCharButton,
		&QPushButton::clicked,
		this,
		[this] () {
			emit addCharacter();
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
		// The second line is the header and also ignored, so decrement again and be at -2
		m_tableWidget->setRowCount(rowOfData.size() - 2);
		// Ignore stored header
		for (int x = 1; x < rowOfData.size() - 1; x++) {
			rowData = rowOfData.at(x).split(";");
			// Create the widget items for the table
			for (int y = 0; y < 6; y++) {
				m_tableWidget->setItem(x - 1, y, new QTableWidgetItem(rowData[y]));
			}
			// If at the first row (which contains information about round counter and the
			// player on the move), get this data
			if (x == 1) {
				m_rowEntered = rowData[ROW_ENTERED].toInt();
				m_roundCounter = rowData[ROUND_CTR].toInt();
			}
		}
	} else {
		// If no data is provided via csv, set the data according to the vector of chars
		// generated in the character creation
		m_tableWidget->setRowCount(m_char->getCharacters().size());

		for (int i = 0; i < m_char->getCharacters().size(); i++) {
			// Store char stats
			m_tableWidget->setItem(i, COL_NAME, new QTableWidgetItem(m_char->getCharacters().at(i)->name));
			m_tableWidget->setItem(
				i,
				COL_INI,
				new QTableWidgetItem(QString::number(m_char->getCharacters().at(i)->initiative)));
			m_tableWidget->setItem(
				i,
				COL_MODIFIER,
				new QTableWidgetItem(QString::number(m_char->getCharacters().at(i)->modifier)));
			m_tableWidget->setItem(
				i,
				COL_HP,
				new QTableWidgetItem(QString::number(m_char->getCharacters().at(i)->hp)));
			if (m_char->getCharacters().at(i)->isNPC) {
				m_tableWidget->setItem(i, COL_NPC, new QTableWidgetItem("X"));
			} else {
				m_tableWidget->setItem(i, COL_NPC, new QTableWidgetItem(" "));
			}
			m_tableWidget->setItem(
				i,
				COL_ADDITIONAL,
				new QTableWidgetItem(m_char->getCharacters().at(i)->additionalInf));
		}
		m_rowEntered = 0;
		m_roundCounter = 1;
	}
	m_tableWidget->setColumnHidden(COL_INI, !m_isIniShown);
	m_tableWidget->setColumnHidden(COL_MODIFIER, !m_isModifierShown);

	// Set the coluḿns containing the initiative, modifier and NPC values as not editable
	for (int i = 0; i < m_tableWidget->rowCount(); i++) {
		m_tableWidget->item(i, COL_INI)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		m_tableWidget->item(i, COL_MODIFIER)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		m_tableWidget->item(i, COL_NPC)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
}


void
TableWidget::setHeight()
{
	for (int i = 0; i < m_tableWidget->rowCount(); i++) {
		m_height += m_tableWidget->rowHeight(i);
	}
	m_height += HEIGHT_BUFFER;
}


// Set the length of the additional information column
// The length depends on whether the INI and Modifier column are shown
void
TableWidget::setInfoColumnWidth()
{
	auto width = WIDTH_ADDITIONAL;
	if (!m_isIniShown) {
		width += WIDTH_INI;
	}
	if (!m_isModifierShown) {
		width += WIDTH_MODIFIER;
	}
	m_tableWidget->setColumnWidth(COL_ADDITIONAL, width);
}


// Sets round couner number
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

	// Depending on the current index, set the new row value
	if (m_tableWidget->currentIndex().row() < row) {
		newRow = row - 1;
		// Swap the identifiers so they match correctly with their row
		std::iter_swap(m_identifiers.begin() + newRow, m_identifiers.begin() + row);
	} else if (m_tableWidget->currentIndex().row() > row) {
		newRow = row + 1;
		std::iter_swap(m_identifiers.begin() + newRow, m_identifiers.begin() + row);
	} else {
		return;
	}
	// The widget items containing the row data (source and destination)
	QList<QTableWidgetItem *> rowItemSrc, rowItemDest;

	// Take the data of the source and destination row
	for (int col = 0; col < m_tableWidget->columnCount(); ++col) {
		rowItemSrc << m_tableWidget->takeItem(row, col);
		rowItemDest << m_tableWidget->takeItem(newRow, col);
	}
	// Set them in reversed order
	for (int col = 0; col < m_tableWidget->columnCount(); ++col) {
		m_tableWidget->setItem(newRow, col, rowItemSrc.at(col));
		m_tableWidget->setItem(row, col, rowItemDest.at(col));
	}
	// After the drag and drop, the correct entered row has to be highlighted
	for (int i = 0; i < m_identifiers.size(); i++) {
		// Set row containing the matching identifier
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
	if (m_tableWidget->rowCount() == 1) {
		m_tableWidget->removeRow(0);
		return;
	}
	// If a row has been selected, remove this row
	if (m_isRowSelected) {
		// If the deleted row is before the current entered row, move one up
		if (m_tableWidget->currentIndex().row() < m_rowEntered) {
			m_rowEntered--;
		}
		// If the deleted row was the last one in the table, select to the first row
		if (m_tableWidget->currentIndex().row() == m_tableWidget->rowCount() - 1) {
			m_rowEntered = 0;
		}
		m_tableWidget->removeRow(m_tableWidget->currentIndex().row());

		m_isRowSelected = false;
		setRowAndPlayer();

		return;
	}
	auto const reply = QMessageBox::warning(
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
TableWidget::addStatusEffect()
{
	// Open dialog
	auto *const dialog = new StatusEffectDialog(this);
	// Lock until dialog is closed
	if (dialog->exec() == QDialog::Accepted) {
		// If accepted, set text
		auto itemText = m_tableWidget->item(
			m_tableWidget->currentIndex().row(),
			COL_ADDITIONAL)->text();
		m_tableWidget->item(
			m_tableWidget->currentIndex().row(),
			COL_ADDITIONAL)->setText(itemText + dialog->getEffect());
	}
}


void
TableWidget::setRowAndPlayer()
{
	// Select row entered with Return key
	m_tableWidget->selectionModel()->clearSelection();

	// Reset bold text rows to standard font
	for (int i = 0; i < m_tableWidget->rowCount(); i++) {
		if (m_tableWidget->item(i, 0)->font().bold()) {
			for (int j = 0; j < m_tableWidget->columnCount(); j++) {
				m_tableWidget->item(i, j)->setFont(m_defaultFont);
			}
		}
	}
	m_tableWidget->selectRow(m_rowEntered);
	// Highlight selected row with bold fonts
	for (int j = 0; j < m_tableWidget->columnCount(); j++) {
		m_tableWidget->item(m_rowEntered, j)->setFont(m_boldFont);
	}

	// Display current player
	m_currentPlayerLabel->setText("Current: " + m_tableWidget->item(m_rowEntered, 0)->text());
}


void
TableWidget::writeSettings()
{
	QSettings settings("LCM", "LCM");

	settings.beginGroup("TableSettings");
	settings.setValue("INI", m_isIniShown);
	settings.setValue("Modifier", m_isModifierShown);
	settings.endGroup();
}


void
TableWidget::readSettings()
{
	QSettings settings("LCM", "LCM");

	settings.beginGroup("TableSettings");
	// If the application is called for the first time, initiative and
	// modifier are shown per default
	m_isIniShown = settings.value("INI").isValid() ?
		       m_isIniShown = settings.value("INI").toBool() :
				      m_isIniShown = true;
	m_isModifierShown = settings.value("Modifier").isValid() ?
			    m_isModifierShown = settings.value("Modifier").toBool() :
						m_isModifierShown = true;
	settings.endGroup();
}


void
TableWidget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete) {
		removeRow();
	}
	if (event->key() == Qt::Key_Return) {
		// If the current selected row is the last one, reset to the first one
		if (m_rowEntered == m_tableWidget->rowCount() - 1) {
			m_rowEntered = 0;
			m_roundCounter++;
			setRoundCounterData();
			// Otherwise just increment to select the next row
		} else {
			m_rowEntered++;
		}
		m_rowIdentifier = m_identifiers.at(m_rowEntered);
		setRowAndPlayer();
	}
	if (event->key() == Qt::Key_E) {
		if (event->modifiers() == Qt::ControlModifier) {
			addStatusEffect();
		}
	}
	QWidget::keyPressEvent(event);
}


void
TableWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);

	// Map from MainWindow coordinates to the Table Widget coordinates
	if (m_tableWidget->itemAt(m_tableWidget->viewport()->mapFrom(this, event->pos())) != nullptr) {
		auto *const statusEffectAction = menu.addAction(
			"Add Status Effect",
			this,
			[this] () {
				addStatusEffect();
			});
	}

	auto *const optionMenu = menu.addMenu("Options");

	auto *const iniAction = optionMenu->addAction(
		"Show Initiatve",
		this,
		[this] (bool show) {
			m_isIniShown = show;
			m_tableWidget->setColumnHidden(COL_INI, !show);
			setInfoColumnWidth();
		});
	iniAction->setCheckable(true);
	iniAction->setChecked(m_isIniShown);

	auto *const modifierAction = optionMenu->addAction(
		"Show Modifier",
		this,
		[this] (bool show) {
			m_isModifierShown = show;
			m_tableWidget->setColumnHidden(COL_MODIFIER, !show);
			setInfoColumnWidth();
		});
	modifierAction->setCheckable(true);
	modifierAction->setChecked(m_isModifierShown);

	menu.exec(event->globalPos());
}


TableWidget::~TableWidget()
{
	writeSettings();
}
