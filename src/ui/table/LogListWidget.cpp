#include "LogListWidget.hpp"

#include <QListWidgetItem>

using enum LogListWidget::LoggingType;

LogListWidget::LogListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setFixedWidth(250);
}


void
LogListWidget::logSingleValue(LoggingType type, int value)
{
    switch (type) {
    case NAME:
        addNewEntry("Changed name for row " + QString::number(value) + ".");
        return;
    case INI:
        addNewEntry("Set INI for row " + QString::number(value) + ".");
        return;
    case INI_MOD:
        addNewEntry("Set INI modifier for row " + QString::number(value) + ".");
        return;
    case HP:
        addNewEntry("Set HP for row " + QString::number(value) + ".");
        return;
    case INFO_TEXT:
        addNewEntry("Added more info for row " + QString::number(value) + ".");
        return;
    case DUPLICATE:
        addNewEntry("Duplicated row " + QString::number(value) + ".");
        return;
    case INFO_EFFECT:
        addNewEntry("Modified effects for " + QString::number(value) + " Characters.");
        return;
    default:
        return;
    }
}


void
LogListWidget::logConditionalValue(LoggingType type, int value, bool condition)
{
    switch (type) {
    case ALLY:
    {
        const auto appendixString = condition ? "'Ally'." : "'Enemy'.";
        addNewEntry("Set enemy status in row " + QString::number(value) + " to " + appendixString);
        return;
    }
    case COUNT:
    {
        const auto newEntry = condition ? "Added " : "Removed ";
        addNewEntry(newEntry + QString::number(value) + " Character(s).");
        return;
    }
    case MULTIPLE_CHARS:
        addNewEntry(condition ? "Changed HP for " + QString::number(value) + " Characters."
                              : "Changed Info Text for " + QString::number(value) + " Characters.");
        return;
    default:
        return;
    }
}


void
LogListWidget::logTwoValues(LoggingType type, int firstValue, int secondValue)
{
    // Currently, we are using only two possible logging types
    type == SWITCH ? addNewEntry("Switched row " + QString::number(firstValue) + " and " + QString::number(secondValue) + ".")
                   : addNewEntry("Next turn (round " + QString::number(firstValue) + ", row " + QString::number(secondValue) + ").");
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
