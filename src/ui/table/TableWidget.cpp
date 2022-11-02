#include "TableWidget.hpp"

#include <QAction>
#include <QCheckBox>
#include <QContextMenuEvent>
#include <QDebug>
#include <QFont>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QShortcut>
#include <QStringList>
#include <QToolButton>
#include <QVBoxLayout>

#include "AddCharacterDialog.hpp"
#include "DelegateSpinBox.hpp"
#include "RuleSettings.hpp"
#include "StatusEffectDialog.hpp"
#include "TableSettings.hpp"
#include "Utils.hpp"

TableWidget::TableWidget(bool isDataStored, std::shared_ptr<RuleSettings> RuleSettings,
			 int mainWidgetWidth, QString data, QWidget *parent)
	: m_isDataStored(isDataStored), m_ruleSettings(RuleSettings), m_data(data)
{
	m_char = std::make_shared<CharacterHandler>();
	m_tableSettings = std::make_shared<TableSettings>();

	m_tableWidget = new DisabledArrowKeyTable();
	m_tableWidget->setColumnCount(NMBR_COLUMNS);

	QStringList tableHeader;
	tableHeader << tr("Name") << "INI" << "Mod" << "HP" << tr("Is Enemy") << tr("Additional information") << "";

	m_tableWidget->setHorizontalHeaderLabels(tableHeader);
	m_tableWidget->verticalHeader()->setVisible(m_tableSettings->verticalHeaderShown);
	m_tableWidget->setShowGrid(true);
	m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	// Adjust row width to main widget size
	m_tableWidget->setColumnWidth(COL_NAME, mainWidgetWidth * WIDTH_NAME);
	m_tableWidget->setColumnWidth(COL_INI, mainWidgetWidth * WIDTH_INI);
	m_tableWidget->setColumnWidth(COL_MODIFIER, mainWidgetWidth * WIDTH_MODIFIER);
	m_tableWidget->setColumnWidth(COL_HP, mainWidgetWidth * WIDTH_HP);
	m_tableWidget->setColumnWidth(COL_ENEMY, mainWidgetWidth * WIDTH_ENEMY);
	// Last column just gets remaining space
	m_tableWidget->horizontalHeader()->setStretchLastSection(true);
	// The last row just contains the row ids, it does not need to be visible
	m_tableWidget->setColumnHidden(COL_ROW_ID, true);

	// Spinbox for the hp column
	auto *const delegateSpinBox = new DelegateSpinBox(this);
	m_tableWidget->setItemDelegateForColumn(COL_HP, delegateSpinBox);

	auto *const downButton = new QToolButton;
	downButton->setArrowType(Qt::DownArrow);
	downButton->setToolTip(tr("Select next Character (Ctrl + Arrow Down)."));

	auto *const upButton = new QToolButton;
	upButton->setArrowType(Qt::UpArrow);
	upButton->setToolTip(tr("Select previous Character (Ctrl + Arrow Up)."));

	auto *const exitButton = new QPushButton(tr("Return to Main Window"));

	auto *const roundCounterLabel = new QLabel;
	auto *const currentPlayerLabel = new QLabel;

	connect(this, &TableWidget::setCurrentPlayer, this, [this, currentPlayerLabel, roundCounterLabel] {
		if (m_tableWidget->rowCount() == 0) {
			currentPlayerLabel->setText(tr("Current: None"));
			roundCounterLabel->setText(tr("Round 0"));
			return;
		}
		currentPlayerLabel->setText(tr("Current: ") + m_tableWidget->item(m_rowEntered, 0)->text());
	});
	connect(this, &TableWidget::roundCounterSet, this, [this, roundCounterLabel] {
		roundCounterLabel->setText(tr("Round ") + QString::number(m_roundCounter));
	});

	// Create a spacer widget to move the buttons to the right side
	auto *const spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	m_defaultFont.setBold(false);
	m_boldFont.setBold(true);

	// Lower layout
	auto *const lowerLayout = new QHBoxLayout();
	lowerLayout->addWidget(roundCounterLabel);
	lowerLayout->addSpacing(SPACING);
	lowerLayout->addWidget(currentPlayerLabel);
	lowerLayout->addWidget(spacer);
	lowerLayout->addWidget(upButton);
	lowerLayout->addWidget(downButton);
	lowerLayout->addWidget(exitButton);

	auto *const mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(m_tableWidget);
	mainLayout->addLayout(lowerLayout);
	setLayout(mainLayout);

	// Shortcuts
	auto *const deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
	auto *const goUpShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up), this);
	auto *const goDownShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down), this);
	auto *const statusEffectShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_E), this);
	auto *const rerollIniShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_I), this);
	auto *const editCombatShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this);

	connect(m_tableWidget, &QTableWidget::cellEntered, this, &TableWidget::dragAndDrop);
	connect(m_tableWidget, &QTableWidget::cellChanged, this, [this] {
		emit changeOccured();
	});

	connect(upButton, &QPushButton::clicked, this, [this] {
		enteredRowChanged(false);
	});
	connect(downButton, &QPushButton::clicked, this, [this] {
		enteredRowChanged(true);
	});
	connect(exitButton, &QPushButton::clicked, this, [this] {
		emit exit();
	});

	connect(deleteShortcut, &QShortcut::activated, this, &TableWidget::removeRow);
	connect(goUpShortcut, &QShortcut::activated, this, [this] {
		enteredRowChanged(false);
	});
	connect(goDownShortcut, &QShortcut::activated, this, [this]{
		enteredRowChanged(true);
	});
	connect(statusEffectShortcut, &QShortcut::activated, this, &TableWidget::openStatusEffectDialog);
	connect(rerollIniShortcut, &QShortcut::activated, this, &TableWidget::rerollIni);
	connect(editCombatShortcut, &QShortcut::activated, this, &TableWidget::openAddCharacterDialog);
}


void
TableWidget::generateTable()
{
	setData();

	// Create the identifiers for the rows
	for (int i = 0; i < m_tableWidget->rowCount(); i++) {
		m_tableWidget->setItem(i, COL_ROW_ID, new QTableWidgetItem(QString::number(i)));
	}
	m_rowIdentifier = m_tableWidget->item(m_rowEntered, COL_ROW_ID)->text().toInt();

	// Set data for the lower label
	emit roundCounterSet();
	setRowAndPlayer();
	emit tableHeightSet(getHeight());
}


unsigned int
TableWidget::getHeight() const
{
	auto height = 0;
	for (int i = 0; i < m_tableWidget->rowCount(); i++) {
		height += m_tableWidget->rowHeight(i);
	}
	return height + HEIGHT_BUFFER;
}


void
TableWidget::openAddCharacterDialog()
{
	// Resynchronize because the table could have been modified
	Utils::resynchronizeCharacters(m_tableWidget, m_char);
	const auto sizeBeforeDialog = m_char->getCharacters().size();

	auto *const dialog = new AddCharacterDialog(m_ruleSettings, this);
	connect(dialog, &AddCharacterDialog::characterCreated, this, &TableWidget::addCharacter);
	// Lock this widget, wait until Dialog is closed
	if (dialog->exec() == QDialog::Accepted) {
		// Only ask to sort if there are enough chars and additional chars have been added
		if (m_char->getCharacters().size() > 1 && m_char->getCharacters().size() != sizeBeforeDialog) {
			auto const reply = QMessageBox::question(
				this,
				tr("Sort characters?"),
				tr("Do you want to sort the table?"),
				QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes) {
				sortTable();
			}
		}
	}
}


// This function enables drag and drop of table rows
// Which works by switching the values of a row with it's upper or lower "neighbor"
void
TableWidget::dragAndDrop(unsigned int row, unsigned int column)
{
	// New row index
	int newRow;

	// Depending on the current index, set the new row value
	if (m_tableWidget->currentIndex().row() < (int) row) {
		newRow = row - 1;
	} else if (m_tableWidget->currentIndex().row() > (int) row) {
		newRow = row + 1;
	} else {
		return;
	}
	// The widget items containing the row data (source and destination)
	QList<QTableWidgetItem *> rowItemSrc, rowItemDest;
	// Checkbox values are not obtainable by using takeItem, so take the current
	// value and reset the boxes with these
	bool enemySrc, enemyDest;

	// Take the data of the source and destination row
	for (int col = 0; col < m_tableWidget->columnCount(); ++col) {
		if (col == COL_ENEMY) {
			auto * const checkBoxSrc = (QCheckBox *) m_tableWidget->cellWidget(row, col);
			auto * const checkBoxDest = (QCheckBox *) m_tableWidget->cellWidget(newRow, col);

			enemySrc = checkBoxSrc->isChecked();
			enemyDest = checkBoxDest->isChecked();
		}
		rowItemSrc << m_tableWidget->takeItem(row, col);
		rowItemDest << m_tableWidget->takeItem(newRow, col);
	}
	// Set them in reversed order
	for (int col = 0; col < m_tableWidget->columnCount(); ++col) {
		m_tableWidget->setItem(newRow, col, rowItemSrc.at(col));
		m_tableWidget->setItem(row, col, rowItemDest.at(col));
	}
	// Set the checkbox values, if they are not equal (otherwise we would perform
	// unnecessary operations)
	if (enemySrc != enemyDest) {
		setTableCheckBox(newRow, enemySrc);
		setTableCheckBox(row, enemyDest);
	}

	// After the drag and drop, the correct entered row has to be highlighted
	for (int i = 0; i < m_tableWidget->rowCount(); i++) {
		// Set row containing the matching identifier
		if (m_tableWidget->item(i, COL_ROW_ID)->text().toInt() == m_rowIdentifier) {
			m_rowEntered = i;
			break;
		}
	}

	emit changeOccured();
}


void
TableWidget::openStatusEffectDialog()
{
	if (m_tableWidget->rowCount() == 0) {
		return;
	}
	// Open dialog
	auto *const dialog = new StatusEffectDialog(m_ruleSettings, this);
	// Lock until dialog is closed
	if (dialog->exec() == QDialog::Accepted) {
		// If accepted, add status effect text
		auto itemText = m_tableWidget->item(
			m_tableWidget->currentIndex().row(),
			COL_ADDITIONAL)->text();
		if (!itemText.isEmpty() && itemText.back() != " ") {
			itemText += " ";
		}

		m_tableWidget->item(
			m_tableWidget->currentIndex().row(),
			COL_ADDITIONAL)->setText(itemText + dialog->getEffect());
	}
}


void
TableWidget::addCharacter(
	QString name,
	int	ini,
	int	mod,
	int	hp,
	bool	isEnemy,
	QString addInfo)
{
	m_char->storeCharacter(name, ini, mod, hp, isEnemy, addInfo);
	resetNameInfoWidth(name, addInfo);
	generateTable();
}


void
TableWidget::rerollIni()
{
	const auto row = m_tableWidget->currentRow();
	const auto newRolledDice = Utils::rollDice();
	const auto newInitiative = newRolledDice + m_tableWidget->item(row, COL_MODIFIER)->text().toInt();
	m_tableWidget->setItem(row, COL_INI, new QTableWidgetItem(
				       QString::number(newInitiative))
			       );
	// Restore the bold font if the current player gets an ini reroll
	if (row == m_rowEntered) {
		setRowAndPlayer();
	}

	const auto messageString = tr("New initiative value: ") + QString::number(newInitiative) + "<br>" +
				   tr("Rolled dice value: ") + QString::number(newRolledDice) + "<br>" +
				   tr("Modifier: ") + m_tableWidget->item(row, COL_MODIFIER)->text();

	auto const reply = QMessageBox::information(this, tr("Rerolled initiative"), messageString);
}


// Set the data inside the table
void
TableWidget::setData()
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
			for (int y = 0; y < NMBR_COLUMNS - 1; y++) {
				if (y == COL_ENEMY) {
					setTableCheckBox(x - 1, rowData[y] == "true" ? true : false);
				} else {
					m_tableWidget->setItem(x - 1, y, new QTableWidgetItem(rowData[y]));
				}
			}
			// If at the first row (which contains information about round counter and the
			// player on the move), get this data
			if (x == 1) {
				m_rowEntered = rowData[ROW_ENTERED].toInt();
				m_roundCounter = rowData[ROUND_CTR].toInt();
			}
			resetNameInfoWidth(rowData[COL_NAME], rowData[COL_ADDITIONAL]);
		}
		// Readd the tables values to the characters
		Utils::resynchronizeCharacters(m_tableWidget, m_char);
		m_isDataStored = false;
	} else {
		// If no data is provided via csv, set the data according to the vector of chars
		// generated in the character creation
		m_tableWidget->setRowCount(static_cast<int>(m_char->getCharacters().size()));

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
			setTableCheckBox(i, m_char->getCharacters().at(i)->isEnemy);
			m_tableWidget->setItem(
				i,
				COL_ADDITIONAL,
				new QTableWidgetItem(m_char->getCharacters().at(i)->additionalInf));
		}
	}
	m_tableWidget->setColumnHidden(COL_INI, !m_tableSettings->iniShown);
	m_tableWidget->setColumnHidden(COL_MODIFIER, !m_tableSettings->modifierShown);
}


void
TableWidget::sortTable()
{
	Utils::resynchronizeCharacters(m_tableWidget, m_char);
	m_char->sortCharacters(m_ruleSettings->ruleset, m_ruleSettings->rollAutomatical);
	m_rowEntered = 0;
	generateTable();
}


void
TableWidget::setRowAndPlayer()
{
	// Select row entered with Return key
	m_tableWidget->selectionModel()->clearSelection();

	if (m_tableWidget->rowCount() != 0) {
		// Reset bold text rows to standard font
		for (int i = 0; i < m_tableWidget->rowCount(); i++) {
			if (m_tableWidget->item(i, 0)->font().bold()) {
				for (int j = 0; j < m_tableWidget->columnCount(); j++) {
					if (j != COL_ENEMY) {
						m_tableWidget->item(i, j)->setFont(m_defaultFont);
					}
				}
			}
		}

		// Highlight selected row with bold fonts
		for (int j = 0; j < m_tableWidget->columnCount(); j++) {
			if (j != COL_ENEMY) {
				m_tableWidget->item(m_rowEntered, j)->setFont(m_boldFont);
			}
		}
	}

	emit setCurrentPlayer();
}


// Remove a row/character of the table
void
TableWidget::removeRow()
{
	if (m_tableWidget->rowCount() == 0) {
		return;
	}

	// If a row has been selected, remove this row
	if (m_tableWidget->selectionModel()->hasSelection()) {
		// If the deleted row is before the current entered row, move one up
		if (m_tableWidget->currentIndex().row() < (int) m_rowEntered) {
			m_rowEntered--;
		}
		// If the deleted row was the last one in the table and also the current player, select to the first row
		if (m_tableWidget->currentIndex().row() == m_tableWidget->rowCount() - 1) {
			if (m_tableWidget->item(m_tableWidget->currentIndex().row(), 0)->font() == m_boldFont) {
				m_rowEntered = 0;
			}
		}
		m_tableWidget->removeRow(m_tableWidget->currentIndex().row());
		// Update the current player and row
		setRowAndPlayer();

		return;
	}

	auto const reply = QMessageBox::warning(
		this,
		tr("Could not remove Character!"),
		tr("Please select a Character with the Mouse Key before deleting!"));
}


void
TableWidget::enteredRowChanged(bool goDown)
{
	if (m_tableWidget->rowCount() == 0) {
		return;
	}

	// Are we going down or up?
	if (goDown) {
		// If the current selected row is the last one, reset to the first row
		if (m_rowEntered == m_tableWidget->rowCount() - 1) {
			m_rowEntered = 0;
			m_roundCounter++;
			emit roundCounterSet();
			// Otherwise just select the next row
		} else {
			m_rowEntered++;
		}
	} else {
		// Stop if first round and first char selected
		if (m_rowEntered == 0 && m_roundCounter == 1) {
			return;
		}
		if (m_rowEntered == 0) {
			m_rowEntered = m_tableWidget->rowCount() - 1;
			m_roundCounter--;
			emit roundCounterSet();
		} else {
			m_rowEntered--;
		}
	}

	// Identifier for the entered row changes
	m_rowIdentifier = m_tableWidget->item(m_rowEntered, COL_ROW_ID)->text().toInt();
	setRowAndPlayer();
}


void
TableWidget::showTablePart(bool show, int valueType)
{
	switch (valueType) {
	case 0:
		m_tableWidget->setColumnHidden(COL_INI, !show);
		m_tableSettings->write(show, TableSettings::ValueType::INI_SHOWN);
		break;
	case 1:
		m_tableWidget->setColumnHidden(COL_MODIFIER, !show);
		m_tableSettings->write(show, TableSettings::ValueType::MOD_SHOWN);
		break;
	case 2:
		m_tableWidget->verticalHeader()->setVisible(show);
		m_tableSettings->write(show, TableSettings::ValueType::VERT_HEADER_SHOWN);
		break;
	default:
		break;
	}
}


// Create checkboxes to show the enemy status
void
TableWidget::setTableCheckBox(unsigned int row, bool checked)
{
	auto *const checkBox = new QCheckBox;
	checkBox->setChecked(checked);
	connect(checkBox, &QCheckBox::stateChanged, this, [this] {
		emit changeOccured();
	});

	// Center the checkboxes
	auto *const widget = new QWidget;
	auto *layout = new QHBoxLayout(widget);
	layout->addWidget(checkBox);
	layout->setAlignment(Qt::AlignCenter);
	widget->setLayout(layout);

	m_tableWidget->setCellWidget(row, COL_ENEMY, widget);
}


// Set a new width for the name and/or additional info column if longer strings are used
void
TableWidget::resetNameInfoWidth(const QString& name, const QString& addInfo)
{
	auto changeOccured = false;
	// Use the bold font for longer columns
	const auto nameWidth = Utils::getStringWidth(name, m_boldFont);
	// Due to the stretch property, the additional info column will be shortened if the name column
	// is enhanced. To prevent this, we store the old value and reuse it if necessary
	auto addInfoWidth = m_tableWidget->columnWidth(COL_ADDITIONAL);

	if (m_tableWidget->columnWidth(COL_NAME) < nameWidth) {
		m_tableWidget->setColumnWidth(COL_NAME, nameWidth + COL_LENGTH_NAME_BUFFER);
		changeOccured = true;
	}
	// additional info might be empty, so possible skip
	if (!addInfo.isEmpty()) {
		const auto addInfoNewWidth = Utils::getStringWidth(addInfo, m_boldFont);
		if (addInfoWidth < addInfoNewWidth) {
			// Reuse the old value if the new string is too short
			addInfoWidth = addInfoNewWidth + COL_LENGTH_ADD_BUFFER;
			changeOccured = true;
		}
	}

	if (changeOccured) {
		// Change the main window width
		auto mainWidth = 0;
		// Ignore the stretchable and indentifier column
		for (int i = 0; i < m_tableWidget->columnCount() - 2; i++) {
			mainWidth += m_tableWidget->columnWidth(i);
		}
		// The main window will adjust and the additional info column will be lenghtened
		emit tableWidthSet(mainWidth + addInfoWidth + COL_LENGTH_ADD_BUFFER);
	}
}


void
TableWidget::contextMenuEvent(QContextMenuEvent *event)
{
	auto *const menu = new QMenu(this);

	// Status Effect and remove options only if the cursor is above an item
	// Map from MainWindow coordinates to Table Widget coordinates
	if (m_tableWidget->indexAt(m_tableWidget->viewport()->mapFrom(this, event->pos())).row() >= 0) {
		auto *const statusEffectAction = menu->addAction(tr("Add Status Effect(s)..."), this, [this] () {
			openStatusEffectDialog();
		});
		statusEffectAction->setShortcut(Qt::CTRL + Qt::Key_E);
		statusEffectAction->setShortcutVisibleInContextMenu(true);

		auto *const rerollIniAction = menu->addAction(tr("Reroll Initiative"), this, &TableWidget::rerollIni);
		rerollIniAction->setShortcut(Qt::CTRL + Qt::Key_I);
		rerollIniAction->setShortcutVisibleInContextMenu(true);

		auto *const removeRowAction = menu->addAction(tr("Remove Character"), this, [this] () {
			removeRow();
		});
		removeRowAction->setShortcut(Qt::Key_Delete);
		removeRowAction->setShortcutVisibleInContextMenu(true);

		menu->addSeparator();
	}

	if (m_tableWidget->rowCount() > 1) {
		auto *const resortAction = menu->addAction(tr("Resort Table"), this, [this] () {
			sortTable();
		});
		menu->addSeparator();
	}

	auto *const openAddCharacterDialogAction = menu->addAction(tr("Add new Character(s)..."), this, [this] () {
		openAddCharacterDialog();
	});
	openAddCharacterDialogAction->setShortcut(Qt::CTRL + Qt::Key_R);
	openAddCharacterDialogAction->setShortcutVisibleInContextMenu(true);

	menu->addSeparator();

	auto *const optionMenu = menu->addMenu("Options");

	auto *const iniAction = optionMenu->addAction(tr("Show Initiative"), this, [this] (bool show) {
		showTablePart(show, 0);
	});
	iniAction->setCheckable(true);
	iniAction->setChecked(m_tableSettings->iniShown);

	auto *const modifierAction = optionMenu->addAction(tr("Show Modifier"), this, [this] (bool show) {
		showTablePart(show, 1);
	});
	modifierAction->setCheckable(true);
	modifierAction->setChecked(m_tableSettings->modifierShown);

	auto *const verticalHeaderAction = optionMenu->addAction(tr("Show Vertical Header"), this, [this] (bool show) {
		showTablePart(show, 2);
	});
	verticalHeaderAction->setCheckable(true);
	verticalHeaderAction->setChecked(m_tableSettings->verticalHeaderShown);

	menu->exec(event->globalPos());
}
