#include "../include/Utils.hpp"

#include <QTableWidget>

#include "GUI/Table/CustomTable.hpp"

Utils::Utils()
{
}


// Resynchronize the char handler vector. This function is called
// if an existing combat is reedited
void
Utils::resynchronizeCharacters(QPointer<CustomTable>	tableWidget,
			       CharacterHandlerRef	characterHandler)
{
	// Clear everything, then use the table cells to refill the character handler
	characterHandler->clearCharacters();

	for (int i = 0; i < tableWidget->rowCount(); i++) {
		// Check if the NPC cell is marked with an X
		auto const isNPC = tableWidget->item(i, 4)->text() == "X" ? true : false;

		characterHandler->storeCharacter(
			tableWidget->item(i, 0)->text(),
			tableWidget->item(i, 1)->text().toInt(),
			tableWidget->item(i, 2)->text().toInt(),
			tableWidget->item(i, 3)->text().toInt(),
			isNPC,
			tableWidget->item(i, 5)->text());
	}
}
