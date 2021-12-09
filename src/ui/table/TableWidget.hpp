#pragma once

#include <QPointer>
#include <QString>
#include <QWidget>

#include "../../handler/CharacterHandler.hpp"
#include "CustomTable.hpp"

// This class handles the creation of the table widget
class TableWidget : public QWidget {
	Q_OBJECT

public:
	TableWidget(bool	isDataStored,
		    bool	newCombatStarted,
		    QString	data = "",
		    QWidget *	parent = 0);

	QTableWidget *
	getTableWidget()
	{
		return m_tableWidget;
	}

	int
	getRowEntered()
	{
		return m_rowEntered;
	}

	int
	getRoundCounter()
	{
		return m_roundCounter;
	}

	bool
	isEmpty()
	{
		return m_tableWidget->rowCount() == 0;
	}

	int
	getHeight();

public slots:

	void
	openAddCharacterDialog();

signals:

	void
	exit();

	void
	tableSet(int height);

	void
	setCurrentPlayer();

	void
	roundCounterSet();

private slots:

	void
	dragAndDrop(int row,
		    int column);

	void
	rowSelected();

	void
	openStatusEffectDialog();

	void
	addCharacter(QString	name,
		     int	ini,
		     int	mod,
		     int	hp,
		     bool	isEnemy,
		     QString	addInfo);

private:

	void
	setTable();

	void
	setData();

	void
	setInfoColumnWidth();

	void
	setRowAndPlayer();

	void
	removeRow();

	void
	enteredRowChanged(bool goDown);

	void
	showIniColumn(bool show);

	void
	showModColumn(bool show);

	void
	writeSettings();

	void
	readSettings();

	void
	contextMenuEvent(QContextMenuEvent *event) override;

	// Main table widget
	QPointer<CustomTable> m_tableWidget;

	// Fonts for highlighting rows
	QFont m_defaultFont;
	QFont m_boldFont;

	bool m_isRowSelected{ false };

	bool m_isDataStored;
	bool m_newCombatStarted;

	QString m_data;

	int m_rowEntered = 0;
	int m_roundCounter = 1;

	bool m_isIniShown{ true };
	bool m_isModifierShown{ true };

	CharacterHandlerRef m_char;

	// The row identifier to determine the correct row after drag and drop
	int m_rowIdentifier = 0;

	static constexpr int NMBR_COLUMNS = 6;

	static constexpr int COL_NAME = 0;
	static constexpr int COL_INI = 1;
	static constexpr int COL_MODIFIER = 2;
	static constexpr int COL_HP = 3;
	static constexpr int COL_ENEMY = 4;
	static constexpr int COL_ADDITIONAL = 5;
	static constexpr int COL_ROW_ID = 6;

	static constexpr int ROW_ENTERED = 6;
	static constexpr int ROUND_CTR = 7;

	static constexpr int SPACING = 30;

	// Width rations of the first five columns
	static constexpr float WIDTH_NAME = 0.25f;
	static constexpr float WIDTH_INI = 0.05f;
	static constexpr float WIDTH_MODIFIER = 0.05f;
	static constexpr float WIDTH_HP = 0.05f;
	static constexpr float WIDTH_ENEMY = 0.12f;

	static constexpr int HEIGHT_BUFFER = 140;
};
