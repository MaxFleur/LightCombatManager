#pragma once

#include <QPointer>
#include <QString>
#include <QWidget>

#include "../../handler/CharacterHandler.hpp"
#include "CustomTable.hpp"

class QContextMenuEvent;
class QFont;
class QHBoxLayout;
class QLabel;
class QTableWidget;
class QVBoxLayout;
class QPushButton;

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

	int
	getRowCount()
	{
		return m_tableWidget->rowCount();
	}

	int
	getHeight()
	{
		return setHeight();
	}

public slots:

	void
	openAddCharacterDialog();

signals:

	void
	exit();

	void
	tableSet(int height);

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
		     bool	isNPC,
		     QString	addInfo);

private:

	void
	setTable();

	void
	setData();

	int
	setHeight();

	void
	setInfoColumnWidth();

	void
	incrementRoundCounter();

	void
	setRoundCounterData();

	void
	setRowAndPlayer();

	void
	removeRow();

	void
	enteredRowChanged();

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

	// Widgets
	QPointer<QPushButton> m_exitButton;
	QPointer<QLabel> m_roundCounterLabel;
	QPointer<QLabel> m_currentPlayerLabel;

	QAction *iniAction;

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

	// Row identifiers
	std::vector<int> m_identifiers;

	// The row identifier to determine the correct row after drag and drop
	int m_rowIdentifier = 0;

	static constexpr int NMBR_COLUMNS = 6;

	static constexpr int COL_NAME = 0;
	static constexpr int COL_INI = 1;
	static constexpr int COL_MODIFIER = 2;
	static constexpr int COL_HP = 3;
	static constexpr int COL_NPC = 4;
	static constexpr int COL_ADDITIONAL = 5;
	static constexpr int ROW_ENTERED = 6;
	static constexpr int ROUND_CTR = 7;

	static constexpr int SPACING = 30;

	static constexpr int WIDTH_NAME = 150;
	static constexpr int WIDTH_INI = 40;
	static constexpr int WIDTH_MODIFIER = 40;
	static constexpr int WIDTH_HP = 35;
	static constexpr int WIDTH_NPC = 60;
	static constexpr int WIDTH_ADDITIONAL = 335;

	static constexpr int HEIGHT_BUFFER = 140;
};
