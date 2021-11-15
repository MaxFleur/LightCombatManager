#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

#include "WelcomeWidget.hpp"
#include "table/TableWidget.hpp"
#include "../handler/CharacterHandler.hpp"
#include "../handler/FileHandler.hpp"

class QAction;

// This class handles the creation and managing of the main GUI window. It also serves
// as "main anchor point", switching between the different widgets the Combat Manager can have.
// The widgets are created in other classes and given to this class.
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

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
	aboutQt();

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

	QPointer<QAction> m_newCombatAction;
	QPointer<QAction> m_saveAction;
	QPointer<QAction> m_openAction;
	QPointer<QAction> m_aboutAction;
	QPointer<QAction> m_aboutQtAction;

	bool m_isTableActive{ false };

	QString m_currentDir;

	FileHandlerRef m_file;
};
#endif
