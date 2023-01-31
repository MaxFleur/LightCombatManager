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
    Undo(const QVector<QVector<QVariant> >& tableDataOld,
         const QVector<int>&                identifiersOld,
         int                                rowEnteredOld,
         int                                roundCounterOld,
         CombatWidget*              CombatWidget,
         const QVector<QVector<QVariant> >& tableDataNew,
         const QVector<int>&                identifiersNew,
         unsigned int*                      rowIdentifier,
         unsigned int*                      rowEntered,
         unsigned int*                      roundCounter,
         QLabel*                            roundCounterLabel,
         QLabel*                            currentPlayerLabel);

    void
    undo() override;

    void
    redo() override;

private:

    void
    setCombatWidget(bool undo);

private:
    QPointer<CombatWidget> m_combatWidget;

    const QVector<QVector<QVariant> > m_tableDataOld;
    const QVector<int> m_identifiersOld;
    int m_rowEnteredOld;
    int m_roundCounterOld;

    const QVector<QVector<QVariant> > m_tableDataNew;
    const QVector<int> m_identifiersNew;

    unsigned int *m_rowIdentifier;
    unsigned int *m_rowEntered;
    unsigned int *m_roundCounter;

    QPointer<QLabel> m_roundCounterLabel;
    QPointer<QLabel> m_currentPlayerLabel;

    static constexpr int COL_NAME = 0;
    static constexpr int COL_ENEMY = 4;
};
