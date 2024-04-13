#pragma once

#include <QVariant>

class CombatWidget;

// Utility functions for the Combat Table
namespace Utils::Table
{
void
setTableAdditionalInfoWidget(CombatWidget*   combatWidget,
                             unsigned int    row,
                             const QVariant& additionalInfo);

static constexpr int COL_NAME = 0;
static constexpr int COL_INI = 1;
static constexpr int COL_MODIFIER = 2;
static constexpr int COL_HP = 3;
static constexpr int COL_ENEMY = 4;
static constexpr int COL_ADDITIONAL = 5;
}
