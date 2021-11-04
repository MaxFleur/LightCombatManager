#include "Utils.hpp"

#include "GUI/Table/CustomTable.hpp"

// Resynchronize the char handler vector. This function is called
// if a table is loaded and/or new characters are readded to a combat
void
Utils::resynchronizeCharacters(QPointer<CustomTable>	tableWidget,
			       CharacterHandlerRef	characterHandler)
{
	// Clear everything, then use the table cells to refill the character handler
	characterHandler->clearCharacters();

	for (int i = 0; i < tableWidget->rowCount(); i++) {
		characterHandler->storeCharacter(
			tableWidget->item(i, 0)->text(),
			tableWidget->item(i, 1)->text().toInt(),
			tableWidget->item(i, 2)->text().toInt(),
			tableWidget->item(i, 3)->text().toInt(),
			// Check if the NPC cell is marked with an X
			tableWidget->item(i, 4)->text() == "X" ? true : false,
			tableWidget->item(i, 5)->text());
	}
}


// If a user manages a Combat, it is likely that value are separated using a semicolon
// But the file storing uses semicoli to separate the values for the csv
// So encapsulate user entered semicoli in double quotes if the table is reloaded
QString
Utils::replaceCharacters(QString str, bool isSaving)
{
	isSaving ? str.replace(';', '";"') : str.replace('";"', ';');
	return str;
}
