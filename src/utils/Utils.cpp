#include "Utils.hpp"

#include <random>

#include <QCheckBox>
#include <QFileInfo>
#include <QFont>
#include <QFontMetrics>
#include <QTableWidget>

namespace Utils
{
// Resynchronize the char handler vector. This function is called
// if a table is loaded and/or new characters are readded to a combat
void
resynchronizeCharacters(const QTableWidget *	tableWidget,
			CharacterHandlerRef	characterHandler)
{
	// Clear everything, then use the table cells to refill the character handler
	characterHandler->clearCharacters();

	for (int i = 0; i < tableWidget->rowCount(); i++) {
		// Cell widget is a checkbox within another widget, so find the child
		auto *const checkBox = tableWidget->cellWidget(i, 4)->findChild<QCheckBox *>();

		characterHandler->storeCharacter(
			tableWidget->item(i, 0)->text(),
			tableWidget->item(i, 1)->text().toInt(),
			tableWidget->item(i, 2)->text().toInt(),
			tableWidget->item(i, 3)->text().toInt(),
			checkBox->isChecked() ? true : false,
			tableWidget->item(i, 5)->text());
	}
}


// If a user manages a Combat, it is likely that value are separated using a semicolon
// But the file storing uses semicolons to separate the values for the csv
// So the name and additional info columns are checked for semicolons
bool
containsSemicolon(const QTableWidget *tableWidget)
{
	for (int i = 0; i < tableWidget->rowCount(); i++) {
		for (int j = 0; j < tableWidget->columnCount(); j++) {
			if (j == 0 || j == 5) {
				if (tableWidget->item(i, j)->text().contains(';')) {
					return true;
				}
			}
		}
	}
	return false;
}


// Roll a 20 sided dice
int
rollDice()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(1, 20);

	return distr(gen);
}


int
getStringWidth(const QString& str, const QFont& font)
{
	const QFontMetrics fontMetrics(font);
	return fontMetrics.boundingRect(str).width();
}


QString
getCSVName(const QString& filePath)
{
	QFileInfo fileInfo(filePath);
	return fileInfo.fileName();
}


QString
getRulesetName(unsigned int ruleset)
{
	switch (ruleset) {
	case 0:
		return "Pathfinder 1E/D&D 3.5E";
		break;
	case 1:
		return "Pathfinder 2E";
		break;
	case 2:
		return "D&D 5E";
		break;
	case 3:
		return "D&D 3.0E";
		break;
	case 4:
		return "Starfinder";
		break;
	default:
		return "";
		break;
	}
}


QString
getAutoRollEnabled(unsigned int autoRollEnabled)
{
	return (autoRollEnabled == 1) ? "automatic rolling enabled" : "automatic rolling disabled";
}
}
