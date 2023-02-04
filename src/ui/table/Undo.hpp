#pragma once

#include <QPointer>
#include <QUndoCommand>

class QLabel;
class QTableWidget;

class CombatWidget;

// Manage the main combat table widget creation and recreation
class Undo : public QUndoCommand
{
public:
    struct UndoData {
        const QVector<QVector<QVariant> > tableData;
        const QVector<int>                identifiers;
        const unsigned int                rowEntered;
        const unsigned int                roundCounter;
    };

public:
    Undo(const UndoData&  oldData,
         const UndoData&  newData,
         CombatWidget*    CombatWidget,
         unsigned int*    rowIdentifier,
         QPointer<QLabel> roundCounterLabel,
         QPointer<QLabel> currentPlayerLabel);

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

    unsigned int *m_rowIdentifier;

    QPointer<QLabel> m_roundCounterLabel;
    QPointer<QLabel> m_currentPlayerLabel;

    static constexpr int COL_NAME = 0;
    static constexpr int COL_ENEMY = 4;
};
