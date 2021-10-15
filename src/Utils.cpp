#include "../include/Utils.hpp"

Utils::Utils()
{
}


// Resynchronize the char handler vector. This function is called
// if an existing combat is reedited
void
Utils::resynchronizeCharacters(QPointer<TableWidget>	tableWidget,
			       CharacterHandlerRef	characterHandler)
{
	// Clear everything, then use the table cells to refill the character handler
	characterHandler->clearCharacters();

	for (int i = 0; i < tableWidget->getTableWidget()->rowCount(); i++) {
		// Check if the NPC cell is marked with an X
		auto const isNPC = tableWidget->getTableWidget()->item(i, 4)->text() == "X" ? true : false;

		characterHandler->storeCharacter(
			tableWidget->getTableWidget()->item(i, 0)->text(),
			tableWidget->getTableWidget()->item(i, 1)->text().toInt(),
			tableWidget->getTableWidget()->item(i, 2)->text().toInt(),
			tableWidget->getTableWidget()->item(i, 3)->text().toInt(),
			isNPC,
			tableWidget->getTableWidget()->item(i, 5)->text());
	}
}
