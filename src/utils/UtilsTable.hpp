#pragma once

#include <QVariant>

class CombatWidget;

// Utility functions for the Combat Table
namespace Utils::Table
{
// Create checkboxes in the table to show the is enemy status
void
setTableCheckBox(CombatWidget *CombatWidget,
                 unsigned int  row,
                 bool          checked);

void
setTableAdditionalInfoWidget(CombatWidget*   combatWidget,
                             unsigned int    row,
                             const QVariant& additionalInfo);

static constexpr int COL_ENEMY = 4;
static constexpr int COL_ADDITIONAL = 5;
}
