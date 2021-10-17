#pragma once

#include <QPointer>

#include "CharacterHandler.hpp"

class CustomTable;
class QTableWidget;

// Utility functions
class Utils {
public:

	Utils();

	static void
	resynchronizeCharacters(QPointer<CustomTable>	tableWidget,
				CharacterHandlerRef	characterHandler);
};
