#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QPointer>

#include "WelcomeWidget.hpp"
#include "CharacterCreationWidget.hpp"
#include "Table/TableWidget.hpp"
#include "../CharacterHandler.hpp"
#include "../FileHandler.hpp"
#include "../Utils.hpp"

// This class handles the creation and managing of the main GUI window (the dropdown menu and it's actions).
// The layouts themselves are created in other classes and given to this class.
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

private slots:

	void
	newCombat();

	void
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

	void
	addCharacterToCombat();

	virtual void
	closeEvent(QCloseEvent *event);

private:

	void
	setWelcomingWidget();

	void
	setCharacterCreationWidget(bool isEditMode);

	void
	setTableWidget(bool	isDataStored,
		       QString	data = "");

	QPointer<WelcomeWidget> m_welcomeWidget;
	QPointer<CharacterCreationWidget> m_characterCreationWidget;
	QPointer<TableWidget> m_tableWidget;

	QPointer<QAction> m_newCombatAction;
	QPointer<QAction> m_saveAction;
	QPointer<QAction> m_openAction;
	QPointer<QAction> m_aboutAction;
	QPointer<QAction> m_aboutQtAction;

	bool m_isCreationActive = false;
	bool m_isTableActive = false;

	CharacterHandlerRef m_char;
	FileHandlerRef m_file;
};
#endif
