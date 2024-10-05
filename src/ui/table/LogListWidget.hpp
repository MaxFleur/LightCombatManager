#pragma once

#include <QListWidget>

// List widget used to display the combat log
class LogListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit
    LogListWidget(QWidget *parent = nullptr);

public:
    void
    logCharacterStatChanged(int  charRow,
                            int  charColumn,
                            // Call only for default
                            bool isAlly = false);

    void
    logChangedCharacterCount(int  count,
                             bool added);

    void
    logChangedHPMultipleChars(int count);

    void
    logCharacterDuplicated(int row);

    void
    logCharacterSwitch(int oldIndex,
                       int newIndex);

    void
    logAddedEffects(int count);

    void
    logNextTurn(int roundCounter,
                int row);

    void
    logOther(const QString& log);

private:
    void
    addNewEntry(const QString& newEntry,
                bool           abortCondition = true);

private:
    static constexpr int TOP_ROW = 0;
};
