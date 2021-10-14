#pragma once

#include <iostream>

#include <QPointer>
#include <QString>
#include <QWidget>

#include "../../CharacterHandler.hpp"
#include "CustomTable.hpp"
#include "Delegate.hpp"

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
	TableWidget(CharacterHandlerRef charHandler,
		    bool		isDataStored,
		    QString		data = "",
		    QWidget *		parent = 0);

	~TableWidget();

	QTableWidget *
	getTableWidget()
	{
		return m_tableWidget;
	}

	// Return the height of this widget;
	int
	getHeight()
	{
		return m_height;
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

signals:

	void
	exit();

	void
	addCharacter();

private slots:

	void
	dragAndDrop(int row,
		    int column);

	void
	removeRow();

	void
	rowSelected();

private:
	CharacterHandlerRef m_char;

	void
	setTableData();

	void
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
	writeSettings();

	void
	readSettings();

	void
	keyPressEvent(QKeyEvent *event);

	void
	contextMenuEvent(QContextMenuEvent *event) override;

	// Main table widget
	QPointer<CustomTable> m_tableWidget;

	// Widgets
	QPointer<QPushButton> m_exitButton;
	QPointer<QPushButton> m_addCharButton;
	QPointer<QLabel> m_roundCounterLabel;
	QPointer<QLabel> m_currentPlayerLabel;

	QAction *iniAction;

	// Fonts for highlighting rows
	QFont m_defaultFont;
	QFont m_boldFont;

	bool m_isRowSelected = false;

	int m_height = 0;

	bool m_isDataStored;

	QString m_data;

	int m_rowEntered;

	int m_roundCounter;

	bool m_isIniShown = true;
	bool m_isModifierShown = true;

	// Row identifiers
	std::vector<int> m_identifiers;

	// The row identifier to determine the correct row after drag and drop
	int m_rowIdentifier;

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
