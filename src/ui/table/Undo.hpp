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
        const QVector<QVector<QVariant> > tableData{};
        const unsigned int                rowEntered{ 0 };
        const unsigned int                roundCounter{ 0 };
    };

public:
    Undo(CombatWidget*    CombatWidget,
         QPointer<QLabel> roundCounterLabel,
         QPointer<QLabel> currentPlayerLabel,
         const UndoData&  oldData,
         const UndoData&  newData,
         unsigned int*    rowEntered,
         unsigned int*    roundCounter,
         bool             colorTableRows,
         bool             showIniToolTips);

    void
    undo() override;

    void
    redo() override;

private:

    void
    setCombatWidget(bool undo);

private:
    QPointer<CombatWidget> m_combatWidget;

    QPointer<QLabel> m_roundCounterLabel;
    QPointer<QLabel> m_currentPlayerLabel;

    const UndoData m_oldData;
    const UndoData m_newData;

    unsigned int *m_rowEntered;
    unsigned int *m_roundCounter;

    const bool m_colorTableRows;
    const bool m_showIniToolTips;

    static constexpr int COL_NAME = 0;
    static constexpr int COL_ENEMY = 4;
    static constexpr int COL_ADDITIONAL = 5;
};
