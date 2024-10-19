#pragma once

#include <QListWidget>

// List widget used to display the combat log
class LogListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit
    LogListWidget(QWidget *parent = nullptr);

    enum class LogType {
        CHANGED_MULTIPLE_HP,
        DUPLICATED_CHAR,
        ADDED_EFFECT,
        CHANGED_CHAR_COUNT,
        CHARS_SWITCHED,
        NEXT_TURN,
        STAT_CHANGED
    };

public:
    void
    log(const LogType logType,
        const int     firstValue,
        const int     secondValue = 0,
        const bool    isAlly = false);

    void
    log(const QString& log);

private:
    void
    addNewEntry(const QString& newEntry,
                bool           abortCondition = true);

private:
    static constexpr int TOP_ROW = 0;
};
