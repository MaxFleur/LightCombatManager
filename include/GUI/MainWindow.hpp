#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

#include "WelcomeWidget.hpp"
#include "CharacterCreationWidget.hpp"
#include "Table/TableWidget.hpp"
#include "../CharacterHandler.hpp"
#include "../FileHandler.hpp"

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

	bool
	saveTable();

	void
	openTable();

	void
	about();

	void
	aboutQt();

	void
	exitCombat();

	void
	cancelCharacterCreation();

	void
	finishCharacterCreation();

private:

	void
	setWelcomingWidget();

	void
	setCharacterCreationWidget();

	void
	setTableWidget(bool	isDataStored,
		       QString	data = "");
    
    void
	writeSettings(QString fileName);

	void
	readSettings();

	void
	closeEvent(QCloseEvent *event);

	QPointer<WelcomeWidget> m_welcomeWidget;
	QPointer<CharacterCreationWidget> m_characterCreationWidget;
	QPointer<TableWidget> m_tableWidget;

	QPointer<QAction> m_newCombatAction;
	QPointer<QAction> m_saveAction;
	QPointer<QAction> m_openAction;
	QPointer<QAction> m_aboutAction;
	QPointer<QAction> m_aboutQtAction;
    
    // QPointer<QSettings> m_settings;

	bool m_isCreationActive = false;
	bool m_isTableActive = false;
    
    QString m_currentDir;

	CharacterHandlerRef m_char;
	FileHandlerRef m_file;
};
#endif
