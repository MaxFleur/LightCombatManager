#pragma once

#include <QPointer>
#include <QUndoCommand>

class QLabel;
class QTableWidget;

class TableWidget;

class Undo : public QUndoCommand
{
public:
	Undo(const QVector<QVector<QVariant> >& tableDataOld,
	     const QVector<int>&		identifiersOld,
	     int				rowEnteredOld,
	     int				roundCounterOld,
	     TableWidget *			tableWidget,
	     const QVector<QVector<QVariant> >& tableDataNew,
	     const QVector<int>&		identifiersNew,
	     int				rowEnteredNew,
	     int				roundCounterNew,
	     unsigned int *			rowIdentifier,
	     QLabel *				roundCounterLabel,
	     QLabel *				currentPlayerLabel);

	void
	undo() override;

	void
	redo() override;

private:

	void
	setCombatWidget(bool undo);

private:
	QPointer<TableWidget> m_tableWidget;

	const QVector<QVector<QVariant> > m_tableDataOld;
	const QVector<int> m_identifiersOld;
	int m_rowEnteredOld;
	int m_roundCounterOld;

	const QVector<QVector<QVariant> > m_tableDataNew;
	const QVector<int> m_identifiersNew;
	int m_rowEnteredNew;
	int m_roundCounterNew;

	unsigned int *m_rowIdentifier;
	QPointer<QLabel> m_roundCounterLabel;
	QPointer<QLabel> m_currentPlayerLabel;

	static constexpr int COL_NAME = 0;
	static constexpr int COL_ENEMY = 4;
};
