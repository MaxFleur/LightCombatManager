#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

#include "../handler/FileHandler.hpp"

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

	int
	saveTable();

	void
	openTable();

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
	writeSettings(QString fileName);

	void
	readSettings();

	void
	closeEvent(QCloseEvent *event);

	QPointer<WelcomeWidget> m_welcomeWidget;
	QPointer<TableWidget> m_tableWidget;

	bool m_isTableActive{ false };

	QString m_currentDir;

	FileHandlerRef m_file;

	static constexpr int START_WIDTH = 720;
	static constexpr int START_HEIGHT = 240;
};
#endif
