#pragma once

#include <QPointer>

#include "GUI/Table/TableWidget.hpp"
#include "CharacterHandler.hpp"

// Utility functions
class Utils {
public:

	Utils();

	static void
	resynchronizeCharacters(QPointer<TableWidget>	tableWidget,
				CharacterHandlerRef	characterHandler);
};
