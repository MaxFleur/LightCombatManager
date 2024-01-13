#include "Undo.hpp"

#include "CombatWidget.hpp"
#include "CombatTableWidget.hpp"
#include "UtilsTable.hpp"

#include <QLabel>
#include <QObject>

Undo::Undo(CombatWidget *CombatWidget, QPointer<QLabel> roundCounterLabel, QPointer<QLabel> currentPlayerLabel,
           const UndoData& oldData, const UndoData& newData,
           unsigned int* rowEntered, unsigned int* roundCounter,
           bool colorTableRows, bool showIniToolTips) :
    m_combatWidget(CombatWidget), m_roundCounterLabel(roundCounterLabel), m_currentPlayerLabel(currentPlayerLabel),
    m_oldData(std::move(oldData)), m_newData(std::move(newData)),
    m_rowEntered(rowEntered), m_roundCounter(roundCounter),
    m_colorTableRows(colorTableRows), m_showIniToolTips(showIniToolTips)
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
    auto *const tableWidget = m_combatWidget->getCombatTableWidget();
    const auto& undoData = undo ? m_oldData : m_newData;

    // Creating the table widget items will trigger the item changed signal, which would recall the undo stack
    // So block the signals as long as we are creating the table
    tableWidget->blockSignals(true);
    tableWidget->setRowCount(undoData.tableData.size());

    // Main table creation
    for (auto i = 0; i < undoData.tableData.size(); ++i) {
        for (auto j = 0; j < undoData.tableData.at(i).size(); ++j) {
            switch (j) {
            case COL_ENEMY:
                Utils::Table::setTableCheckBox(m_combatWidget, i, undoData.tableData.at(i).at(j).toBool());
                break;
            case COL_ADDITIONAL:
                Utils::Table::setTableAdditionalInfoWidget(m_combatWidget, i, undoData.tableData.at(i).at(j));
                break;
            default:
                tableWidget->setItem(i, j, new QTableWidgetItem(undoData.tableData.at(i).at(j).toString()));
                break;
            }
        }
    }

    // Set values for the labels
    if (tableWidget->rowCount() > 0) {
        *m_rowEntered = undoData.rowEntered;
        *m_roundCounter = undoData.roundCounter;
    }

    // Set the remaining label and font data
    m_roundCounterLabel->setText(QObject::tr("Round ") + QString::number(undoData.roundCounter));
    tableWidget->setRowAndPlayer(m_roundCounterLabel, m_currentPlayerLabel, undoData.rowEntered);
    tableWidget->setTableRowColor(!m_colorTableRows);
    tableWidget->setIniColumnTooltips(!m_showIniToolTips);

    emit m_combatWidget->tableHeightSet(tableWidget->getHeight());
    emit m_combatWidget->changeOccured();

    tableWidget->blockSignals(false);
}
