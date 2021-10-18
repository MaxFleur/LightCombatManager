#pragma once

#include <QPointer>

#include "CharacterHandler.hpp"

class CustomTable;

// Utility functions
class Utils {
public:

	static void
	resynchronizeCharacters(QPointer<CustomTable>	tableWidget,
				CharacterHandlerRef	characterHandler);
};
