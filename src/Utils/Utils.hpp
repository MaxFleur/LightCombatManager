#pragma once

#include <QPointer>

#include "../Handler/CharacterHandler.hpp"

class CustomTable;

// Utility functions, no ctor needed
class Utils {
public:

	static void
	resynchronizeCharacters(QPointer<CustomTable>	tableWidget,
				CharacterHandlerRef	characterHandler);

	static QString
	replaceCharacters(QString	str,
			  bool		isSaving);
};
