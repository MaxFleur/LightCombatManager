#pragma once

#include <QPointer>
#include <QString>
#include <QWidget>

#include "CharacterHandler.hpp"
#include "DisabledArrowKeyTable.hpp"

class MainSettings;
class TableSettings;

// This class handles the creation of the table widget
class TableWidget : public QWidget {
	Q_OBJECT

public:
	TableWidget(bool				isDataStored,
		    std::shared_ptr<MainSettings>	MainSettings,
		    QString				data = "",
		    QWidget *				parent = 0);

	void
	generateTable();

	[[nodiscard]] QTableWidget *
	getTableWidget() const
	{
		return m_tableWidget;
	}

	[[nodiscard]] int
	getRowEntered() const
	{
		return m_rowEntered;
	}

	[[nodiscard]] int
	getRoundCounter() const
	{
		return m_roundCounter;
	}

	[[nodiscard]] bool
	isEmpty() const
	{
		return m_tableWidget->rowCount() == 0;
	}

	[[nodiscard]] int
	getHeight() const;

public slots:

	void
	openAddCharacterDialog();

signals:

	void
	exit();

	void
	tableWidthSet(int width);

	void
	tableHeightSet(int height);

	void
	characterNotSelected();

	void
	setCurrentPlayer();

	void
	roundCounterSet();

	void
	changeOccured();

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
	setTableCheckBox(int	row,
			 bool	checked);

	void
	resetNameInfoWidth(QString	strName,
			   QString	strAdd);

	void
	writeSettings();

	void
	readSettings();

	void
	contextMenuEvent(QContextMenuEvent *event) override;

	// Main table widget
	QPointer<DisabledArrowKeyTable> m_tableWidget;

	// Fonts for highlighting rows
	QFont m_defaultFont;
	QFont m_boldFont;

	bool m_isRowSelected{ false };

	bool m_isDataStored;

	QString m_data;

	int m_rowEntered = 0;
	int m_roundCounter = 1;

	CharacterHandlerRef m_char;
	std::shared_ptr<MainSettings> m_mainSettings;
	std::shared_ptr<TableSettings> m_tableSettings;

	// The row identifier to determine the correct row after drag and drop
	int m_rowIdentifier = 0;

	static constexpr int NMBR_COLUMNS = 7;

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

	static constexpr float COL_LENGTH_NAME_BUFFER = 10;
	static constexpr float COL_LENGTH_ADD_BUFFER = 20;

	// Width rations of the first five columns
	static constexpr float WIDTH_NAME = 0.25f;
	static constexpr float WIDTH_INI = 0.05f;
	static constexpr float WIDTH_MODIFIER = 0.05f;
	static constexpr float WIDTH_HP = 0.11f;
	static constexpr float WIDTH_ENEMY = 0.12f;

	static constexpr int HEIGHT_BUFFER = 140;
};
