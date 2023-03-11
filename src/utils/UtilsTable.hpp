#pragma once

#include <QVariant>
#include <QVector>

#include "CharacterHandler.hpp"

class QLabel;
class QTableWidget;

class CombatWidget;

// Utility functions for the Combat Table
namespace Utils
{
namespace Table
{
// Resynchronize the characters stored in the char handler vector
// with the data in the table widget
void
resynchronizeCharacters(const QTableWidget *tableWidget,
                        CharacterHandlerRef characterHandler);

// Create checkboxes in the table to show the is enemy status
void
setTableCheckBox(CombatWidget *CombatWidget,
                 unsigned int  row,
                 bool          checked);

// Set the labels displaying the current player and round number
void
setRowAndPlayer(QTableWidget *tableWidget,
                QLabel*       roundCounterLabel,
                QLabel*       currentPlayerLabel,
                int           rowEntered,
                int           roundCounter);

// Store the table cell values in a vector
[[nodiscard]] QVector<QVector<QVariant> >
tableDataFromWidget(const QTableWidget *tableWidget);

// Reformat the character vector in a QVariant vector
[[nodiscard]] QVector<QVector<QVariant> >
tableDataFromCharacterVector(CharacterHandlerRef characterHandler);

static constexpr int COL_NAME = 0;
static constexpr int COL_ENEMY = 4;
}
}
