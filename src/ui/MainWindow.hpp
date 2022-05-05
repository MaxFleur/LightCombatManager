#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

#include "FileHandler.hpp"

class QMessageBox;

class DirSettings;
class RuleSettings;
class TableWidget;
class WelcomeWidget;

// This class handles the creation and managing of the main GUI window. It also serves
// as "main anchor point", switching between the different widgets the Combat Manager can have.
// The widgets are created in other classes and given to this class.
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

signals:

	void
	setSaveAction(bool enable);

private slots:

	void
	newCombat();

	bool
	saveTable();

	void
	saveAs();

	void
	openTable();

	void
	openSettings();

	void
	about();

	void
	exitCombat();

private:

	void
	setWelcomingWidget();

	void
	setTableWidget(bool		isDataStored,
		       bool		newCombatStarted,
		       const QString&	data = "");

	void
	setCombatTitle(bool	isCombatActive,
		       bool	newCombat = false);

	[[nodiscard]] int
	createSaveMessageBox(const QString&	tableMessage,
			     bool		isClosing);

	[[nodiscard]] QMessageBox *
	createRuleChangeMessageBox();

	void
	closeEvent(QCloseEvent *event);

	[[nodiscard]] bool
	checkStoredTableRules(QString data);

	QPointer<WelcomeWidget> m_welcomeWidget;
	QPointer<TableWidget> m_tableWidget;

	bool m_isTableActive{ false };
	bool m_tableInFile{ false };

	QString m_fileName{ "" };

	FileHandlerRef m_file;
	std::shared_ptr<RuleSettings> m_ruleSettings;
	std::shared_ptr<DirSettings> m_dirSettings;

	RuleSettings::Ruleset m_loadedTableRule;
	bool m_loadedTableRollAutomatically;

	static constexpr int START_WIDTH = 720;
	static constexpr int START_HEIGHT = 240;

	static constexpr int COL_RULESET = 8;
	static constexpr int COL_ROLL_AUTOMATICALLY = 9;
};
#endif
