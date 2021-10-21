#include "../include/Utils.hpp"

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
