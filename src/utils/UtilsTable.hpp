#pragma once

#include "CharacterHandler.hpp"

class QLabel;
class QTableWidget;

class CombatWidget;

// Utility functions for the Combat Table
namespace Utils::Table
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

void
setTableAdditionalInfoWidget(CombatWidget*   combatWidget,
                             unsigned int    row,
                             const QVariant& additionalInfo);

// Set the labels displaying the current player and round number
void
setRowAndPlayer(QTableWidget *tableWidget,
                QLabel*       roundCounterLabel,
                QLabel*       currentPlayerLabel,
                int           rowEntered);

void
setTableRowColor(QTableWidget *tableWidget,
                 bool          resetColor);

void
setStatusEffectInWidget(const QTableWidget *                      tableWidget,
                        QVector<AdditionalInfoData::StatusEffect> statusEffects,
                        int                                       row);

void
adjustStatusEffectRoundCounter(const QTableWidget *tableWidget,
                               bool                decrease);

// Store the table cell values in a vector
[[nodiscard]] QVector<QVector<QVariant> >
tableDataFromWidget(const QTableWidget *tableWidget);

// Reformat the character vector in a QVariant vector
[[nodiscard]] QVector<QVector<QVariant> >
tableDataFromCharacterVector(CharacterHandlerRef characterHandler);

static constexpr int COL_NAME = 0;
static constexpr int COL_ENEMY = 4;
static constexpr int COL_ADDITIONAL = 5;
static constexpr int FIRST_FOUR_COLUMNS = 4;
}
