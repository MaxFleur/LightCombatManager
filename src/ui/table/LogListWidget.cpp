#include "LogListWidget.hpp"

#include <QListWidgetItem>

LogListWidget::LogListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setFixedWidth(250);
}


void
LogListWidget::log(const LogType logType, const int firstValue, const int secondValue, const bool isAlly)
{
    switch (logType) {
    case LogType::CHANGED_MULTIPLE_HP:
        addNewEntry("Changed HP for " + QString::number(firstValue) + " Character(s).");
        break;
    case LogType::DUPLICATED_CHAR:
        addNewEntry("Duplicated row " + QString::number(firstValue) + ".");
        break;
    case LogType::ADDED_EFFECT:
        addNewEntry("Modified effects for " + QString::number(firstValue) + " Character(s).");
        break;
    case LogType::CHANGED_CHAR_COUNT:
    {
        auto frontString = secondValue == 1 ? "Added " : "Removed ";
        addNewEntry(frontString + QString::number(firstValue) + " Character(s).");
        break;
    }
    case LogType::CHARS_SWITCHED:
        addNewEntry("Switched row " + QString::number(firstValue) + " and " + QString::number(secondValue) + ".");
        break;
    case LogType::NEXT_TURN:
        addNewEntry("Next turn (round " + QString::number(firstValue) + ", row " + QString::number(secondValue) + ").");
        break;

    case LogType::STAT_CHANGED:
        switch (secondValue) {
        case 0:
            addNewEntry("Changed name for row " + QString::number(firstValue) + ".");
            break;
        case 1:
            addNewEntry("Set INI for row " + QString::number(firstValue) + ".");
            break;
        case 2:
            addNewEntry("Set INI modifier for row " + QString::number(firstValue) + ".");
            break;
        case 3:
            addNewEntry("Set HP for row " + QString::number(firstValue) + ".");
            break;
        case 5:
            addNewEntry("Added more info for row " + QString::number(firstValue) + ".");
            break;
        default:
            const auto appendixString = isAlly ? "'Ally'." : "'Enemy'.";
            addNewEntry("Set enemy status in row " + QString::number(firstValue) + " to " + appendixString);
            break;
        }
    }
}


void
LogListWidget::log(const QString& log)
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
