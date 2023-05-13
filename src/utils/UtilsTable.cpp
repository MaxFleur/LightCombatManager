#include "UtilsTable.hpp"

#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QTableWidget>
#include <QWidget>

#include "CombatWidget.hpp"

namespace Utils
{
namespace Table
{
void
resynchronizeCharacters(const QTableWidget *tableWidget,
                        CharacterHandlerRef characterHandler)
{
    characterHandler->clearCharacters();

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        // Cell widget is a checkbox within another widget, so find the child
        auto *const checkBox = tableWidget->cellWidget(i, 4)->findChild<QCheckBox *>();

        characterHandler->storeCharacter(
            tableWidget->item(i, 0)->text(),
            tableWidget->item(i, 1)->text().toInt(),
            tableWidget->item(i, 2)->text().toInt(),
            tableWidget->item(i, 3)->text().toInt(),
            checkBox->isChecked(),
            tableWidget->item(i, 5)->text());
    }
}


void
setTableCheckBox(CombatWidget *combatWidget, unsigned int row, bool checked)
{
    auto *const tableWidget = combatWidget->getTableWidget();

    auto *const checkBox = new QCheckBox;
    checkBox->setChecked(checked);
    QObject::connect(checkBox, &QCheckBox::stateChanged, tableWidget, [combatWidget, tableWidget, checkBox] {
                tableWidget->blockSignals(true);
                // We need to store the old checkbox state, so we will reset the state for a short time
                // Then, after saving, reset to the new value and set the correct undo command
                checkBox->blockSignals(true);
                checkBox->setChecked(checkBox->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
                combatWidget->saveOldState();
                checkBox->setChecked(checkBox->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
                combatWidget->pushOnUndoStack(true);
                checkBox->blockSignals(false);

                emit combatWidget->changeOccured();
                tableWidget->blockSignals(false);
            });

    // Center the checkboxes
    auto *const widget = new QWidget;
    auto *layout = new QHBoxLayout(widget);
    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);
    widget->setLayout(layout);

    tableWidget->setCellWidget(row, COL_ENEMY, widget);
}


void
setRowAndPlayer(QTableWidget *tableWidget, QLabel *roundCounterLabel, QLabel *currentPlayerLabel, int rowEntered, int roundCounter)
{
    tableWidget->selectionModel()->clearSelection();
    // Setting fonts may trigger unwished item setting events, so block the signals
    tableWidget->blockSignals(true);
    if (tableWidget->rowCount() != 0) {
        // Reset bold text rows to standard font
        for (int i = 0; i < tableWidget->rowCount(); i++) {
            if (tableWidget->item(i, 0)->font().bold()) {
                const auto font = tableWidget->item(i, 0)->font();

                for (int j = 0; j < tableWidget->columnCount(); j++) {
                    if (j != COL_ENEMY) {
                        tableWidget->item(i, j)->setFont(font.defaultFamily());
                    }
                }
            }
        }

        // Highlight selected row with bold fonts
        for (int j = 0; j < tableWidget->columnCount(); j++) {
            auto font = tableWidget->item(rowEntered, 0)->font();
            font.setBold(true);

            if (j != COL_ENEMY) {
                tableWidget->item(rowEntered, j)->setFont(font);
            }
        }
    }
    tableWidget->blockSignals(false);

    if (tableWidget->rowCount() == 0) {
        currentPlayerLabel->setText(QObject::tr("Current: None"));
        roundCounterLabel->setText(QObject::tr("Round 0"));
        return;
    }
    currentPlayerLabel->setText(QObject::tr("Current: ") + tableWidget->item(rowEntered, 0)->text());
}


void
setTableRowColor(QTableWidget *tableWidget, bool resetColor)
{
    tableWidget->blockSignals(true);

    const auto color = [](bool resetColor, bool isEnemy) {
                           if (resetColor) {
                               return QApplication::palette().color(QPalette::Base);
                           } else {
                               return isEnemy ? QColor(255, 194, 10, 75) : QColor(12, 123, 220, 75);
                           }
                       };

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        const auto isEnemy = tableWidget->cellWidget(i, COL_ENEMY)->findChild<QCheckBox *>()->isChecked();
        for (int j = 0; j < tableWidget->columnCount(); j++) {
            if (j != COL_ENEMY) {
                tableWidget->item(i, j)->setBackground(color(resetColor, isEnemy));
            } else {
                QPalette palette;
                palette.setColor(QPalette::Base, color(resetColor, isEnemy));
                tableWidget->cellWidget(i, COL_ENEMY)->setAutoFillBackground(!resetColor);
                tableWidget->cellWidget(i, COL_ENEMY)->setPalette(palette);
            }
        }
    }

    tableWidget->blockSignals(false);
}


QVector<QVector<QVariant> >
tableDataFromWidget(const QTableWidget *tableWidget)
{
    QVector<QVector<QVariant> > tableData;
    for (int i = 0; i < tableWidget->rowCount(); i++) {
        QVector<QVariant> rowValues;

        for (int j = 0; j < tableWidget->columnCount(); j++) {
            j == COL_ENEMY ?
            rowValues.push_back(tableWidget->cellWidget(i, j)->findChild<QCheckBox *>()->isChecked()) :
            rowValues.push_back(tableWidget->item(i, j)->text());
        }
        tableData.push_back(rowValues);
    }

    return tableData;
}


QVector<QVector<QVariant> >
tableDataFromCharacterVector(CharacterHandlerRef characterHandler)
{
    const auto& characters = characterHandler->getCharacters();

    QVector<QVector<QVariant> > tableData;
    for (int i = 0; i < characters.size(); i++) {
        QVector<QVariant> charValues;

        charValues << characters.at(i).name << characters.at(i).initiative << characters.at(i).modifier
                   << characters.at(i).hp << characters.at(i).isEnemy << characters.at(i).additionalInformation;

        tableData.push_back(charValues);
    }

    return tableData;
}
}
}
