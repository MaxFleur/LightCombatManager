#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

#include "../handler/FileHandler.hpp"

class MainSettings;
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
	saveCombat();

	void
	loadCombat();

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
	setTableWidget(bool	isDataStored,
		       bool	newCombatStarted,
		       QString	data = "");

	void
	setCombatTitle();

	void
	writeSettings(QString	fileName,
		      bool	setSaveDir);

	void
	readSettings();

	void
	closeEvent(QCloseEvent *event);

	void
	handleSubDir();

	QPointer<WelcomeWidget> m_welcomeWidget;
	QPointer<TableWidget> m_tableWidget;

	bool m_isTableActive{ false };
	bool m_changeOccured{ false };
	bool m_tableInFile{ false };

	QString m_openDir;
	QString m_saveDir;

	FileHandlerRef m_file;
	std::shared_ptr<MainSettings> m_mainSettings;

	static constexpr int START_WIDTH = 720;
	static constexpr int START_HEIGHT = 240;
};
#endif
