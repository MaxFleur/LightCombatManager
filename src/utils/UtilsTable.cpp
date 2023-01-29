#include "UtilsTable.hpp"

#include <QCheckBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QTableWidget>
#include <QWidget>

#include "TableWidget.hpp"

namespace Utils
{
namespace Table
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
			checkBox->isChecked(),
			tableWidget->item(i, 5)->text());
	}
}


// Create checkboxes to show the enemy status
void
setTableCheckBox(TableWidget *tableWidget, unsigned int row, bool checked)
{
	auto *const mainTableWidget = tableWidget->getTableWidget();

	auto *const checkBox = new QCheckBox;
	checkBox->setChecked(checked);
	QObject::connect(checkBox, &QCheckBox::stateChanged, tableWidget, [tableWidget, mainTableWidget] {
				mainTableWidget->blockSignals(true);
				emit tableWidget->changeOccured();
				mainTableWidget->blockSignals(false);
			});

	// Center the checkboxes
	auto *const widget = new QWidget;
	auto *layout = new QHBoxLayout(widget);
	layout->addWidget(checkBox);
	layout->setAlignment(Qt::AlignCenter);
	widget->setLayout(layout);

	mainTableWidget->setCellWidget(row, COL_ENEMY, widget);
}


void
setRowAndPlayer(QTableWidget *tableWidget, QLabel *roundCounterLabel, QLabel *currentPlayerLabel, int rowEntered, int roundCounter)
{
	// Select row entered with Return key
	tableWidget->selectionModel()->clearSelection();

	tableWidget->blockSignals(true);
	if (tableWidget->rowCount() != 0) {
		// Reset bold text rows to standard font
		for (int i = 0; i < tableWidget->rowCount(); i++) {
			if (tableWidget->item(i, 0)->font().bold()) {
				const auto font = tableWidget->item(i, 0)->font();

				for (int j = 0; j < tableWidget->columnCount(); j++) {
					if (j != COL_ENEMY) {
						tableWidget->item(i, j)->setFont(font.defaultFamily());
					}
				}
			}
		}

		// Highlight selected row with bold fonts
		for (int j = 0; j < tableWidget->columnCount(); j++) {
			auto font = tableWidget->item(rowEntered, 0)->font();
			font.setBold(true);

			if (j != COL_ENEMY) {
				tableWidget->item(rowEntered, j)->setFont(font);
			}
		}
	}
	tableWidget->blockSignals(false);

	if (tableWidget->rowCount() == 0) {
		currentPlayerLabel->setText(QObject::tr("Current: None"));
		roundCounterLabel->setText(QObject::tr("Round 0"));
		return;
	}
	currentPlayerLabel->setText(QObject::tr("Current: ") + tableWidget->item(rowEntered, 0)->text());
}


QVector<QVector<QVariant> >
tableDataFromWidget(QTableWidget *tableWidget)
{
	QVector<QVector<QVariant> > tableData;
	for (int i = 0; i < tableWidget->rowCount(); i++) {
		QVector<QVariant> rowValues;

		for (int j = 0; j < tableWidget->columnCount(); j++) {
			j == COL_ENEMY ?
			rowValues.push_back(tableWidget->cellWidget(i, j)->findChild<QCheckBox *>()->isChecked()) :
			rowValues.push_back(tableWidget->item(i, j)->text());
		}
		tableData.push_back(rowValues);
	}

	return tableData;
}


QVector<QVector<QVariant> >
tableDataFromCharacterVector(const QVector<CharacterHandler::Character>& characters)
{
	QVector<QVector<QVariant> > tableData;
	for (int i = 0; i < characters.size(); i++) {
		QVector<QVariant> charValues;

		charValues	<< characters.at(i).name << characters.at(i).initiative << characters.at(i).modifier
				<< characters.at(i).hp << characters.at(i).isEnemy << characters.at(i).additionalInf;

		tableData.push_back(charValues);
	}

	return tableData;
}


QVector<int>
identifiers(QTableWidget *tableWidget)
{
	QVector<int> identifiers;
	for (int i = 0; i < tableWidget->rowCount(); i++) {
		if (!tableWidget->item(i, COL_NAME)) {
			break;
		}
		identifiers.push_back(tableWidget->item(i, COL_NAME)->data(Qt::UserRole).toInt());
	}

	return identifiers;
}
}
}
