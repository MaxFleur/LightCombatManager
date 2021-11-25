#pragma once

#include "../handler/CharacterHandler.hpp"

class QTableWidget;

// Utility functions, no ctor needed
namespace Utils
{
void
resynchronizeCharacters(QTableWidget *		tableWidget,
			CharacterHandlerRef	characterHandler);

bool
containsSemicolon(QTableWidget *tableWidget);
}
