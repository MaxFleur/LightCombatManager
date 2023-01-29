#include "Undo.hpp"

#include <QCheckBox>
#include <QDebug>
#include <QLabel>
#include <QObject>

#include "UtilsTable.hpp"
#include "TableWidget.hpp"

Undo::Undo(const QVector<QVector<QVariant> >& tableDataOld, const QVector<int>& identifiersOld, int rowEnteredOld, int roundCounterOld,
	   TableWidget *tableWidget, const QVector<QVector<QVariant> >& tableDataNew, const QVector<int>& identifiersNew, int rowEnteredNew, int roundCounterNew,
	   unsigned int *rowIdentifier, QLabel *roundCounterLabel, QLabel *currentPlayerLabel) :
	m_tableDataOld(tableDataOld), m_identifiersOld(identifiersOld), m_rowEnteredOld(rowEnteredOld), m_roundCounterOld(roundCounterOld),
	m_tableWidget(tableWidget), m_tableDataNew(tableDataNew), m_identifiersNew(identifiersNew), m_rowEnteredNew(rowEnteredNew), m_roundCounterNew(roundCounterNew),
	m_rowIdentifier(rowIdentifier), m_roundCounterLabel(roundCounterLabel), m_currentPlayerLabel(currentPlayerLabel)
{
}


void
Undo::undo()
{
	setCombatWidget(true);
}


void
Undo::redo()
{
	setCombatWidget(false);
}


void
Undo::setCombatWidget(bool undo)
{
	// Set the table
	auto * const mainTableWidget = m_tableWidget->getTableWidget();
	const auto& tableData = undo ? m_tableDataOld : m_tableDataNew;
	const auto& identifiers = undo ? m_identifiersOld : m_identifiersNew;
	const auto& rowEntered = undo ? m_rowEnteredOld : m_rowEnteredNew;
	const auto& roundCounter = undo ? m_roundCounterOld : m_roundCounterNew;

	mainTableWidget->blockSignals(true);
	mainTableWidget->setRowCount(tableData.size());

	for (int i = 0; i < tableData.size(); ++i) {
		for (int j = 0; j < tableData.at(i).size(); ++j) {
			if (j == COL_ENEMY) {
				Utils::Table::setTableCheckBox(m_tableWidget, i, tableData.at(i).at(j).toBool());
			} else {
				mainTableWidget->setItem(i, j, new QTableWidgetItem(tableData.at(i).at(j).toString()));
			}
		}
	}
	for (int i = 0; i < mainTableWidget->rowCount(); i++) {
		mainTableWidget->item(i, COL_NAME)->setData(Qt::UserRole, QString::number(identifiers.at(i)));
	}

	// Set row identifier
	if (mainTableWidget->rowCount() > 0) {
		*m_rowIdentifier = mainTableWidget->item(rowEntered, COL_NAME)->data(Qt::UserRole).toInt();
	}

	// Set the remaining label and font data
	m_roundCounterLabel->setText(QObject::tr("Round ") + QString::number(roundCounter));
	Utils::Table::setRowAndPlayer(mainTableWidget, m_roundCounterLabel, m_currentPlayerLabel, rowEntered, roundCounter);

	emit m_tableWidget->tableHeightSet(m_tableWidget->getHeight());
	mainTableWidget->blockSignals(false);
}
