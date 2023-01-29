#pragma once

#include "CharacterHandler.hpp"

class QFont;
class QTableWidget;

// Utility functions, no ctor needed
namespace Utils
{
namespace General
{
[[nodiscard]] bool
containsSemicolon(const QTableWidget *tableWidget);

[[nodiscard]] int
rollDice();

[[nodiscard]] int
getStringWidth(const QString&	str,
	       const QFont&	font);

[[nodiscard]] QString
getCSVName(const QString& filePath);

[[nodiscard]] QString
getRulesetName(unsigned int ruleset);

[[nodiscard]] QString
getAutoRollEnabled(unsigned int autoRollEnabled);
}
}
