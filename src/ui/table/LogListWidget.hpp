#pragma once

#include <QListWidget>

// List widget used to display the combat log
class LogListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit
    LogListWidget(QWidget *parent = nullptr);

    enum class LoggingType {
        NAME,
        INI,
        INI_MOD,
        HP,
        ALLY,
        INFO_TEXT,
        INFO_EFFECT,
        COUNT,
        MULTIPLE_CHARS,
        DUPLICATE,
        SWITCH,
        NEXT_TURN,
        OTHER
    };

public:
    void
    logSingleValue(LoggingType type,
                   int         value);

    void
    logConditionalValue(LoggingType type,
                        int         value,
                        // Call only for default
                        bool        condition = false);

    void
    logTwoValues(LoggingType type,
                 int         firstValue,
                 int         secondValue);

    void
    logOther(const QString& log);

private:
    void
    addNewEntry(const QString& newEntry,
                bool           abortCondition = true);

private:
    static constexpr int TOP_ROW = 0;
};
