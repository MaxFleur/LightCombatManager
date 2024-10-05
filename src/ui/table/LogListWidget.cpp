#include "LogListWidget.hpp"

#include <QListWidgetItem>

LogListWidget::LogListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setFixedWidth(250);
}


void
LogListWidget::logCharacterStatChanged(int charRow, int charColumn, bool isAlly)
{
    QString listEntry;

    switch (charColumn) {
    case 0:
        listEntry = "Changed name ";
        break;
    case 1:
        listEntry = "Set INI ";
        break;
    case 2:
        listEntry = "Set INI modifier ";
        break;
    case 3:
        listEntry = "Set HP ";
        break;
    case 5:
        listEntry = "Added more info ";
        break;
    default:
        const auto appendixString = isAlly ? "'Ally'." : "'Enemy.'";
        addNewEntry("Set enemy status in row " + QString::number(charRow) + " to " + appendixString);
        return;
    }

    addNewEntry(listEntry += "for row " + QString::number(charRow) + ".");
}


void
LogListWidget::logChangedCharacterCount(int count, bool added, bool isUndoing)
{
    QString newEntry;

    if (isUndoing) {
        newEntry = "UNDO - ";
    }
    newEntry += added ? "Added " : "Removed ";
    addNewEntry(newEntry + QString::number(count) + " Character(s).");
}


void
LogListWidget::logChangedHPMultipleChars(int count)
{
    addNewEntry("Changed HP for " + QString::number(count) + " Characters.");
}


void
LogListWidget::logCharacterDuplicated(int row)
{
    addNewEntry("Duplicated row " + QString::number(row) + ".");
}


void
LogListWidget::logCharacterSwitch(int oldIndex, int newIndex)
{
    addNewEntry("Switched row " + QString::number(oldIndex) + " and " + QString::number(newIndex) + ".");
}


void
LogListWidget::logAddedEffects(int count)
{
    addNewEntry("Modified effects for " + QString::number(count) + " Characters.");
}


void
LogListWidget::logNextTurn(int roundCounter, int row)
{
    addNewEntry("Next turn (round " + QString::number(roundCounter) + ", row " + QString::number(row) + ").");
}


void
LogListWidget::logOther(const QString& log)
{
    addNewEntry(log);
}


void
LogListWidget::addNewEntry(const QString& newEntry)
{
    auto* const listWidgetItem = new QListWidgetItem(newEntry);
    listWidgetItem->setFlags(listWidgetItem->flags() & ~Qt::ItemIsSelectable);

    insertItem(TOP_ROW, listWidgetItem);
}
