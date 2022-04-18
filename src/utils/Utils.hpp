#pragma once

#include "CharacterHandler.hpp"

class QFont;
class QTableWidget;

// Utility functions, no ctor needed
namespace Utils
{
void
resynchronizeCharacters(QTableWidget *		tableWidget,
			CharacterHandlerRef	characterHandler);

[[nodiscard]] bool
containsSemicolon(QTableWidget *tableWidget);

[[nodiscard]] int
rollDice();

[[nodiscard]] int
getStringWidth(QString	str,
	       QFont	font);
}
