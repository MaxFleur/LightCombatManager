#include "Undo.hpp"

#include <QCheckBox>
#include <QDebug>
#include <QLabel>
#include <QObject>

#include "CombatWidget.hpp"
#include "UtilsTable.hpp"

Undo::Undo(const QVector<QVector<QVariant> >& tableDataOld, const QVector<int>& identifiersOld, int rowEnteredOld, int roundCounterOld,
           CombatWidget *CombatWidget, const QVector<QVector<QVariant> >& tableDataNew, const QVector<int>& identifiersNew,
           unsigned int *rowIdentifier, unsigned int *rowEntered, unsigned int *roundCounter, QLabel *roundCounterLabel, QLabel *currentPlayerLabel) :
    m_tableDataOld(tableDataOld), m_identifiersOld(identifiersOld), m_rowEnteredOld(rowEnteredOld), m_roundCounterOld(roundCounterOld),
    m_combatWidget(CombatWidget), m_tableDataNew(tableDataNew), m_identifiersNew(identifiersNew),
    m_rowIdentifier(rowIdentifier), m_rowEntered(rowEntered), m_roundCounter(roundCounter),
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
    auto * const tableWidget = m_combatWidget->getTableWidget();
    const auto& tableData = undo ? m_tableDataOld : m_tableDataNew;
    const auto& identifiers = undo ? m_identifiersOld : m_identifiersNew;
    const auto& rowEntered = undo ? m_rowEnteredOld : *m_rowEntered;
    const auto& roundCounter = undo ? m_roundCounterOld : *m_roundCounter;

    // Creating the table widget items will trigger the item changed signal, which would recall the undo stack
    // So block the signals as long as we are creating the table
    tableWidget->blockSignals(true);
    tableWidget->setRowCount(tableData.size());

    for (int i = 0; i < tableData.size(); ++i) {
        for (int j = 0; j < tableData.at(i).size(); ++j) {
            if (j == COL_ENEMY) {
                Utils::Table::setTableCheckBox(m_combatWidget, i, tableData.at(i).at(j).toBool());
            } else {
                tableWidget->setItem(i, j, new QTableWidgetItem(tableData.at(i).at(j).toString()));
            }
        }
    }
    for (int i = 0; i < tableWidget->rowCount(); i++) {
        tableWidget->item(i, COL_NAME)->setData(Qt::UserRole, QString::number(identifiers.at(i)));
    }

    // Set values for the labels
    if (tableWidget->rowCount() > 0 && undo) {
        *m_rowIdentifier = tableWidget->item(rowEntered, COL_NAME)->data(Qt::UserRole).toInt();
        *m_rowEntered = rowEntered;
        *m_roundCounter = roundCounter;
    }

    // Set the remaining label and font data
    m_roundCounterLabel->setText(QObject::tr("Round ") + QString::number(roundCounter));
    Utils::Table::setRowAndPlayer(tableWidget, m_roundCounterLabel, m_currentPlayerLabel, rowEntered, roundCounter);

    emit m_combatWidget->tableHeightSet(m_combatWidget->getHeight());

    tableWidget->blockSignals(false);
}
