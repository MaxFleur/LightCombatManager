#include "Undo.hpp"

#include <QCheckBox>
#include <QDebug>
#include <QLabel>
#include <QObject>

#include "CombatWidget.hpp"
#include "UtilsTable.hpp"

Undo::Undo(const UndoData& oldData, const UndoData& newData, CombatWidget *CombatWidget, unsigned int *rowIdentifier,
           QPointer<QLabel> roundCounterLabel, QPointer<QLabel> currentPlayerLabel) :
    m_oldData(oldData), m_newData(newData), m_combatWidget(CombatWidget), m_rowIdentifier(rowIdentifier),
    m_roundCounterLabel(roundCounterLabel), m_currentPlayerLabel(currentPlayerLabel)
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
    // Set with old or new values, depending on if we are undoing or not
    auto *const tableWidget = m_combatWidget->getTableWidget();
    const auto& undoData = undo ? m_oldData : m_newData;

    // Creating the table widget items will trigger the item changed signal, which would recall the undo stack
    // So block the signals as long as we are creating the table
    tableWidget->blockSignals(true);
    tableWidget->setRowCount(undoData.tableData.size());

    for (int i = 0; i < undoData.tableData.size(); ++i) {
        for (int j = 0; j < undoData.tableData.at(i).size(); ++j) {
            j == COL_ENEMY ?
            Utils::Table::setTableCheckBox(m_combatWidget, i, undoData.tableData.at(i).at(j).toBool()):
            tableWidget->setItem(i, j, new QTableWidgetItem(undoData.tableData.at(i).at(j).toString()));
        }
    }
    for (int i = 0; i < tableWidget->rowCount(); i++) {
        tableWidget->item(i, COL_NAME)->setData(Qt::UserRole, QString::number(undoData.identifiers.at(i)));
    }

    // Set values for the labels
    if (tableWidget->rowCount() > 0 && undo) {
        *m_rowIdentifier = tableWidget->item(undoData.rowEntered, COL_NAME)->data(Qt::UserRole).toInt();
    }

    // Set the remaining label and font data
    m_roundCounterLabel->setText(QObject::tr("Round ") + QString::number(undoData.roundCounter));
    Utils::Table::setRowAndPlayer(tableWidget, m_roundCounterLabel, m_currentPlayerLabel, undoData.rowEntered, undoData.roundCounter);

    emit m_combatWidget->tableHeightSet(m_combatWidget->getHeight());

    tableWidget->blockSignals(false);
}
