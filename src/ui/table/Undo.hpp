#pragma once

#include <QPointer>
#include <QUndoCommand>

class QLabel;
class QTableWidget;

class CombatWidget;

// Manage the main combat table widget undoing and redoing
class Undo : public QUndoCommand
{
public:
    struct UndoData {
        const QVector<QVector<QVariant> > tableData;
        const unsigned int                rowEntered;
        const unsigned int                roundCounter;
    };

public:
    Undo(const UndoData&  oldData,
         const UndoData&  newData,
         CombatWidget*    CombatWidget,
         unsigned int*    rowEntered,
         unsigned int*    roundCounter,
         QPointer<QLabel> roundCounterLabel,
         QPointer<QLabel> currentPlayerLabel,
         bool             colorTableRows);

    void
    undo() override;

    void
    redo() override;

private:

    void
    setCombatWidget(bool undo);

private:
    QPointer<CombatWidget> m_combatWidget;

    const UndoData m_oldData;
    const UndoData m_newData;

    unsigned int *m_rowEntered;
    unsigned int *m_roundCounter;

    QPointer<QLabel> m_roundCounterLabel;
    QPointer<QLabel> m_currentPlayerLabel;

    bool m_colorTableRows;

    static constexpr int COL_NAME = 0;
    static constexpr int COL_ENEMY = 4;
    static constexpr int COL_ADDITIONAL = 5;
};
