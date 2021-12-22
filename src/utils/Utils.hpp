#pragma once

#include "../handler/CharacterHandler.hpp"

class QFont;
class QTableWidget;

// Utility functions, no ctor needed
namespace Utils
{
void
resynchronizeCharacters(QTableWidget *		tableWidget,
			CharacterHandlerRef	characterHandler);

bool
containsSemicolon(QTableWidget *tableWidget);

int
rollDice();

int
getStringWidth(QString	str,
	       QFont	font);
}
