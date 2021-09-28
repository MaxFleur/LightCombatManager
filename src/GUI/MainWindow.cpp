#include "../../include/GUI/MainWindow.hpp"

#include <QAction>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QKeySequence>
#include <QMenuBar>
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow()
{
	// Actions
	m_newCombatAction = new QAction("&New Combat", this);
	m_newCombatAction->setShortcuts(QKeySequence::New);
	m_newCombatAction->setStatusTip("Start a new combat.");
	connect(m_newCombatAction, &QAction::triggered, this, &MainWindow::newCombat);

	m_saveAction = new QAction("&Save Table", this);
	m_saveAction->setShortcuts(QKeySequence::Save);
	m_saveAction->setStatusTip("Save the created table.");
	connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveTable);

	m_openAction = new QAction("&Open Table", this);
	m_openAction->setShortcuts(QKeySequence::Open);
	m_openAction->setStatusTip("Open an existing table.");
	connect(m_openAction, &QAction::triggered, this, &MainWindow::openTable);

	m_aboutAction = new QAction("&About", this);
	m_aboutAction->setStatusTip("About Light Combat Manager");
	connect(m_aboutAction, &QAction::triggered, this, &MainWindow::about);

	m_aboutQtAction = new QAction("About &Qt", this);
	m_aboutQtAction->setStatusTip("About QT version and license");
	connect(m_aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

	// Menus
	auto *fileMenu = menuBar()->addMenu("&File");
	fileMenu->addAction(m_newCombatAction);
	fileMenu->addAction(m_saveAction);
	fileMenu->addAction(m_openAction);
	fileMenu->addSeparator();

	auto *helpMenu = menuBar()->addMenu("&Help");
	helpMenu->addAction(m_aboutAction);
	helpMenu->addAction(m_aboutQtAction);

	m_char = std::make_shared<CharacterHandler>();
	m_file = std::make_shared<FileHandler>();

	setWelcomingWidget();
	setWindowTitle("Light Combat Manager");
}


void
MainWindow::closeEvent(QCloseEvent *event)
{
	// If a game has been started, send a message if the game should be closed
	if (m_isCreationActive) {
		QMessageBox::StandardButton reply = QMessageBox::question(
			this,
			"Exit",
			"You are in the character creation right now! Do you want to exit the program anyway? All stored characters will be lost.",
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			QApplication::exit;
		} else {
			event->ignore();
		}
	}
	// If the table is active, send a question if the table should be saved
	if (m_isTableActive) {
		QMessageBox::StandardButton reply = QMessageBox::question(
			this,
			"Exit",
			"Currently, you are in a combat. Do you want to save the characters before exiting the program?",
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes) {
			saveTable();
		} else if (reply == QMessageBox::Cancel) {
			event->ignore();
		}
		QApplication::exit;
	}
}


void
MainWindow::newCombat()
{
	if (m_isCreationActive) {
		// If a game is currently running, asks if a new combat should be started anyway
		QMessageBox::StandardButton reply = QMessageBox::question(
			this,
			"Start a new combat?",
			"Currently, you are storing characters for a beginning combat. Do you want to create a new combat anyway? "
			"This will delete all characters you have stored for this combat.",
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			m_char->clearCharacters();
		} else {
			return;
		}
		// If the table is active right now, asks if the current table should be saved
	} else if (m_isTableActive) {
		QMessageBox::StandardButton reply = QMessageBox::question(
			this,
			"Start a new combat?",
			"Do you want to save the current combat before starting a new combat?",
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes) {
			saveTable();
		} else if (reply == QMessageBox::Cancel) {
			return;
		}
		m_isTableActive = false;
		m_char->clearCharacters();
	}
	m_isCreationActive = true;
	setCharacterCreationWidget();
}


void
MainWindow::saveTable()
{
	if (mp_tableWidget->getRowCount() < 2) {
		QMessageBox::StandardButton reply = QMessageBox::critical(
			this,
			"Too few table entries!",
			"The table contains less then 2 entries.");
		return;
	}
	QString filename = QFileDialog::getSaveFileName(
		this,
		"Save Table",
		QDir::currentPath(),
		"Table (*.csv);;All Files (*)");

	if (filename.isEmpty()) {
		return;
	}

	m_file->saveTable(
		mp_tableWidget->getTableWidget(),
		filename,
		mp_tableWidget->getRowEntered(),
		mp_tableWidget->getRoundCounter());
}


void
MainWindow::openTable()
{
	if (m_isCreationActive) {
		// If a game is currently running, asks if a table shoud be opened anyway
		QMessageBox::StandardButton reply = QMessageBox::question(
			this,
			"Open table?",
			"Currently, you are storing characters for a beginning combat. Do you want to open another table anyway?",
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			m_char->clearCharacters();
		} else {
			return;
		}
		// If the table is active right now, asks if the current table should be saved
	} else if (m_isTableActive) {
		QMessageBox::StandardButton reply = QMessageBox::question(
			this,
			"Save current table?",
			"Do you want to save the current combat before opening another table?",
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes) {
			saveTable();
		} else if (reply == QMessageBox::Cancel) {
			return;
		}
	}
	QString filename = QFileDialog::getOpenFileName(this, "Open Table", QDir::currentPath(), ("csv File(*.csv)"));

	int code = m_file->getCSVData(filename);

	switch (code) {
	case 0:
	{
		break;
	}
	case 1:
	{
		QMessageBox::StandardButton reply = QMessageBox::critical(
			this,
			"Wrong table format!",
			"The loading of the table failed because the table has the wrong format.");
		break;
	}
	case 2:
		// Table and char creation not active for a short time
		m_isCreationActive = false;
		m_isTableActive = false;
		setTableWidget(true, m_file->getData());
		break;
	}
}


void
MainWindow::about()
{
	QMessageBox::about(
		this,
		"About Light Combat Manager",
		"Light Combat Manager. A simple Combat Manager for DnD-like games. Code available on Github:"
		"\nhttps://github.com/MaxFleur/LightCombatManager"
		"\n"
		"\nVersion 1.0.0.");
}


void
MainWindow::aboutQt()
{
}


void
MainWindow::cancelCharacterCreation()
{
	QMessageBox::StandardButton reply = QMessageBox::question(
		this,
		"Cancel creation?",
		"Are you sure you want to cancel the character creation? All created characters will be lost.",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		m_char->clearCharacters();
		m_isCreationActive = false;
		setWelcomingWidget();
	}
}


void
MainWindow::finishCharacterCreation()
{
	// If no character was stored or just one character was stored and the name field of the char creation widget is empty, abort
	if (m_char->getCharacters().size() == 0 || m_char->getCharacters().size() == 1 &&
	    mp_characterCreationWidget->isNameEmpty()) {
		QMessageBox::StandardButton reply = QMessageBox::warning(
			this,
			"Could not finish!",
			"Please store at least 2 characters before creating the combat table!");
		return;
	}
	// Else, display a question message asking if the creation should be finished
	QMessageBox::StandardButton reply = QMessageBox::question(
		this,
		"Finish creation?",
		"Are you sure you want to finish the character creation? Characters can't be added afterwards.",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		mp_characterCreationWidget->storeLastCharacter();
		m_isCreationActive = false;
		m_char->sortCharacters();
		setTableWidget(false);
	}
}


void
MainWindow::exitCombat()
{
	QMessageBox::StandardButton reply = QMessageBox::question(
		this,
		"Exit combat?",
		"Are you sure you want to exit the combat? All created characters will be lost.",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		m_char->clearCharacters();
		setWelcomingWidget();
		m_isTableActive = false;
	}
}


void
MainWindow::setWelcomingWidget()
{
	mp_welcomeWidget = new WelcomeWidget(this);
	setCentralWidget(mp_welcomeWidget);
	setFixedSize(640, 260);
	m_saveAction->setEnabled(false);
}


void
MainWindow::setCharacterCreationWidget()
{
	mp_characterCreationWidget = new CharacterCreationWidget(m_char, this);
	setCentralWidget(mp_characterCreationWidget);
	connect(
		mp_characterCreationWidget,
		&CharacterCreationWidget::cancel,
		this,
		&MainWindow::cancelCharacterCreation);
	connect(
		mp_characterCreationWidget,
		&CharacterCreationWidget::finish,
		this,
		&MainWindow::finishCharacterCreation);
	setFixedSize(640, 260);
	m_saveAction->setEnabled(false);
}


void
MainWindow::setTableWidget(bool isDataStored, QString data)
{
	mp_tableWidget = new TableWidget(m_char, isDataStored, data, this);
	setCentralWidget(mp_tableWidget);
	connect(mp_tableWidget, &TableWidget::exit, this, &MainWindow::exitCombat);
	m_isTableActive = true;
	setMinimumSize(640, mp_tableWidget->getHeight());
	setMaximumWidth(QWIDGETSIZE_MAX);
	m_saveAction->setEnabled(true);
}


MainWindow::~MainWindow()
{
}
