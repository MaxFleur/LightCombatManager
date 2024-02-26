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

static constexpr int COL_ADDITIONAL = 5;
}
