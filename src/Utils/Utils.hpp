#pragma once

#include <QPointer>

#include "../Handler/CharacterHandler.hpp"

class CustomTable;
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
