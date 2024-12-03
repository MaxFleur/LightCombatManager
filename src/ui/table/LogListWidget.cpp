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
        const auto appendixString = isAlly ? "'Ally'." : "'Enemy'.";
        addNewEntry("Set enemy status in row " + QString::number(charRow) + " to " + appendixString);
        return;
    }

    addNewEntry(listEntry += "for row " + QString::number(charRow) + ".");
}


void
LogListWidget::logChangedCharacterCount(int count, bool added)
{
    auto newEntry = added ? "Added " : "Removed ";
    addNewEntry(newEntry + QString::number(count) + " Character(s).");
}


void
LogListWidget::logChangedStatMultipleChars(int count, bool hpChanged)
{
    addNewEntry(hpChanged ? "Changed HP for " + QString::number(count) + " Characters."
                          : "Changed Info Text for " + QString::number(count) + " Characters.");
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
    addNewEntry(log, log == "Redo called.");
}


void
LogListWidget::addNewEntry(const QString& newEntry, bool abortCondition)
{
    // @note
    // QUndoStack makes it difficult to supervise when a redo operation was performed manually, so we have to improvise it.
    // Thus we forbid logging a redo until an undo command has been performed.
    static auto abortRedoLogging = false;
    if (newEntry != "Redo called.") {
        abortRedoLogging = abortCondition;
    }
    if (newEntry == "Redo called." && abortRedoLogging) {
        return;
    }

    auto* const listWidgetItem = new QListWidgetItem(newEntry);
    listWidgetItem->setFlags(listWidgetItem->flags() & ~Qt::ItemIsSelectable);

    insertItem(TOP_ROW, listWidgetItem);
}
