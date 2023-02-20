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
void
resynchronizeCharacters(const QTableWidget *tableWidget,
                        CharacterHandlerRef characterHandler);

void
setTableCheckBox(CombatWidget *CombatWidget,
                 unsigned int  row,
                 bool          checked);

void
setRowAndPlayer(QTableWidget *tableWidget,
                QLabel*       roundCounterLabel,
                QLabel*       currentPlayerLabel,
                int           rowEntered,
                int           roundCounter);


[[nodiscard]] QVector<QVector<QVariant> >
tableDataFromWidget(QTableWidget *tableWidget);

[[nodiscard]] QVector<QVector<QVariant> >
tableDataFromCharacterVector(const QVector<CharacterHandler::Character>& characters);

static constexpr int COL_NAME = 0;
static constexpr int COL_ENEMY = 4;
}
}
