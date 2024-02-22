#include "Undo.hpp"

#include "CombatWidget.hpp"
#include "CombatTableWidget.hpp"
#include "UtilsTable.hpp"

#include <QLabel>
#include <QObject>

Undo::Undo(CombatWidget *CombatWidget, QPointer<QLabel> roundCounterLabel, QPointer<QLabel> currentPlayerLabel,
           const UndoData& oldData, const UndoData& newData, const std::vector<int> affectedRows,
           unsigned int* rowEntered, unsigned int* roundCounter,
           bool colorTableRows, bool showIniToolTips) :
    m_combatWidget(CombatWidget), m_roundCounterLabel(roundCounterLabel), m_currentPlayerLabel(currentPlayerLabel),
    m_oldData(std::move(oldData)), m_newData(std::move(newData)), m_affectedRows(std::move(affectedRows)),
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
    const auto& oldTableData = m_oldData.tableData;
    const auto& newTableData = m_newData.tableData;

    // Creating the table widget items will trigger the item changed signal, which would recall the undo stack
    // So block the signals as long as we are creating the table
    tableWidget->blockSignals(true);

    // Insert or remove rows if the corresponding operations were called
    if (!m_affectedRows.empty()) {
        adjustTableWidgetRowCount((oldTableData.size() > newTableData.size() && undo) ||
                                  (oldTableData.size() < newTableData.size() && !undo));
    } else {
        // For everything else, we just need to update the changed items
        for (auto row = 0; row < oldTableData.size(); row++) {
            if (oldTableData.at(row) == newTableData.at(row)) {
                continue;
            }

            const auto& rowData = undo ? oldTableData.at(row) : newTableData.at(row);
            for (auto col = 0; col < COL_COUNT; col++) {
                if (oldTableData.at(row).at(col) == newTableData.at(row).at(col)) {
                    continue;
                }

                fillTableWidgetCell(rowData.at(col), row, col);
            }
        }
    }

    const auto& undoData = undo ? m_oldData : m_newData;
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


void
Undo::fillTableWidgetCell(const QVariant& data, int row, int col)
{
    auto *const tableWidget = m_combatWidget->getCombatTableWidget();

    switch (col) {
    case COL_ENEMY:
        Utils::Table::setTableCheckBox(m_combatWidget, row, data.toBool());
        break;
    case COL_ADDITIONAL:
        Utils::Table::setTableAdditionalInfoWidget(m_combatWidget, row, data);
        break;
    default:
        tableWidget->item(row, col) ? tableWidget->item(row, col)->setText(data.toString())
                                    : tableWidget->setItem(row, col, new QTableWidgetItem(data.toString()));
        break;
    }
}


void
Undo::adjustTableWidgetRowCount(bool addRow)
{
    auto *const tableWidget = m_combatWidget->getCombatTableWidget();
    auto affectedRows = m_affectedRows;
    const auto& oldTableData = m_oldData.tableData;
    const auto& newTableData = m_newData.tableData;

    while (!affectedRows.empty()) {
        const auto front = affectedRows.front();
        addRow ? tableWidget->insertRow(front) : tableWidget->removeRow(front);

        if (addRow) {
            const auto& tableData = oldTableData.size() > newTableData.size() ? oldTableData.at(front) : newTableData.at(front);
            for (auto col = 0; col < COL_COUNT; col++) {
                fillTableWidgetCell(tableData.at(col), front, col);
            }
        } else {
            for (std::size_t i = 0; i < affectedRows.size(); i++) {
                affectedRows[i]--;
            }
        }
        affectedRows.erase(affectedRows.begin());
    }
}
