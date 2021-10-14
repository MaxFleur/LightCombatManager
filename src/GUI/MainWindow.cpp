#include "../../include/GUI/MainWindow.hpp"

#include <QAction>
#include <QApplication>
#include <QDebug>
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
	auto *const fileMenu = menuBar()->addMenu("&File");
	fileMenu->addAction(m_newCombatAction);
	fileMenu->addAction(m_saveAction);
	fileMenu->addAction(m_openAction);
	fileMenu->addSeparator();

	auto *const helpMenu = menuBar()->addMenu("&Help");
	helpMenu->addAction(m_aboutAction);
	helpMenu->addAction(m_aboutQtAction);

	m_char = std::make_shared<CharacterHandler>();
	m_file = std::make_shared<FileHandler>();

	setWelcomingWidget();
}


void
MainWindow::newCombat()
{
	// If a creation is currently running, asks if a new combat should be started anyway
	if (m_isCreationActive) {
		auto const reply = QMessageBox::question(
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
		auto const reply = QMessageBox::question(
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
	setCharacterCreationWidget(false);
}


int
MainWindow::saveTable()
{
	if (m_tableWidget->getRowCount() < 2) {
		// Too few rows to save
		return 2;
	}
	auto const filename = QFileDialog::getSaveFileName(
		this,
		"Save Table",
		QDir::currentPath(),
		"Table (*.csv);;All Files (*)");

	if (filename.isEmpty()) {
		// No file provided or Cancel pressed
		return 1;
	}

	m_file->saveTable(
		m_tableWidget->getTableWidget(),
		filename,
		m_tableWidget->getRowEntered(),
		m_tableWidget->getRoundCounter());
	// Success
	return 0;
}


void
MainWindow::openTable()
{
	// If a creation is currently running, asks if a table shoud be opened anyway
	if (m_isCreationActive) {
		auto const reply = QMessageBox::question(
			this,
			"Open table?",
			"Currently, you are storing characters for a beginning combat. Do you want to open another table anyway? "
			"This will delete all characters you have stored for this combat.",
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			m_char->clearCharacters();
		} else {
			return;
		}
		// If the table is active right now, asks if the current table should be saved
	} else if (m_isTableActive) {
		auto const reply = QMessageBox::question(
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
	auto const filename =
		QFileDialog::getOpenFileName(this, "Open Table", QDir::currentPath(), ("csv File(*.csv)"));
	auto const code = m_file->getCSVData(filename);

	switch (code) {
	case 0:
	{
		break;
	}
	case 1:
	{
		auto const reply = QMessageBox::critical(
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
		"<p>Light Combat Manager. A simple Combat Manager for Pathfinder 1e. <br>"
		"<a href='https://github.com/MaxFleur/LightCombatManager'>Code available on Github.</a></p>"
		"<p>Version 1.1.2.</p>");
}


void
MainWindow::aboutQt()
{
}


void
MainWindow::exitCombat()
{
	auto const reply = QMessageBox::question(
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
MainWindow::cancelCharacterCreation()
{
	// Ignore if no character is stored yet
	if (m_char->getCharacters().size() > 0) {
		auto const reply = QMessageBox::question(
			this,
			"Cancel creation?",
			"Are you sure you want to cancel the character creation? All created characters will be lost.",
			QMessageBox::Yes | QMessageBox::No);

		if (reply == QMessageBox::Yes) {
			m_char->clearCharacters();
		} else {
			return;
		}
	}
	m_isCreationActive = false;
	setWelcomingWidget();
}


void
MainWindow::finishCharacterCreation()
{
	// If no or just one character was stored and the name field of the char creation widget is empty, abort
	if (m_char->getCharacters().size() < 2 &&
	    m_characterCreationWidget->isNameEmpty()) {
		auto const reply = QMessageBox::warning(
			this,
			"Could not finish!",
			"Please store at least 2 characters before creating the combat table!");
		return;
	}
	// Display a question message asking if the creation should be finished
	auto const reply = QMessageBox::question(
		this,
		"Finish creation?",
		"Are you sure you want to finish the character creation?",
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		m_characterCreationWidget->storeLastCharacter();
		m_isCreationActive = false;
		m_char->sortCharacters();
		setTableWidget(false);
	}
}


void
MainWindow::addCharacterToCombat()
{
	Utils::resynchronizeCharacters(m_tableWidget, m_char);
	setCharacterCreationWidget(true);
}


void
MainWindow::closeEvent(QCloseEvent *event)
{
	// If the creation is active, ask if it should be closed
	if (m_isCreationActive) {
		auto const reply = QMessageBox::question(
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
		auto const reply = QMessageBox::question(
			this,
			"Exit",
			"Currently, you are in a combat. Do you want to save the characters before exiting the program?",
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes) {
			auto const code = saveTable();
			switch (code) {
			case 0:
			{
				break;
			}
			case 1:
			{
				event->ignore();
				break;
			}
			case 2:
				auto const reply = QMessageBox::critical(
					this,
					"Too few table entries!",
					"The table contains less then 2 entries.");
				event->ignore();
				break;
			}
		} else if (reply == QMessageBox::Cancel) {
			event->ignore();
		}
		QApplication::exit;
	}
}


void
MainWindow::setWelcomingWidget()
{
	m_welcomeWidget = new WelcomeWidget(this);
	setCentralWidget(m_welcomeWidget);
	setFixedSize(700, 260);
	m_saveAction->setEnabled(false);
	setWindowTitle("LCM");
}


void
MainWindow::setCharacterCreationWidget(bool isEditMode)
{
	m_characterCreationWidget = new CharacterCreationWidget(m_char, isEditMode, this);
	setCentralWidget(m_characterCreationWidget);
	setFixedSize(700, 280);
	m_characterCreationWidget->setNameFocus();
	m_saveAction->setEnabled(false);

	connect(
		m_characterCreationWidget,
		&CharacterCreationWidget::cancel,
		this,
		&MainWindow::cancelCharacterCreation);
	connect(
		m_characterCreationWidget,
		&CharacterCreationWidget::finish,
		this,
		&MainWindow::finishCharacterCreation);

	if (isEditMode) {
		setWindowTitle("LCM - Reediting existing Combat");
		return;
	}
	setWindowTitle("LCM - Creating new Combat");
}


void
MainWindow::setTableWidget(bool isDataStored, QString data)
{
	m_tableWidget = new TableWidget(m_char, isDataStored, data, this);
	setCentralWidget(m_tableWidget);
	connect(m_tableWidget, &TableWidget::exit, this, &MainWindow::exitCombat);
	connect(m_tableWidget, &TableWidget::addCharacter, this, &MainWindow::addCharacterToCombat);
	m_isTableActive = true;
	setMinimumSize(700, m_tableWidget->getHeight());
	setMaximumWidth(QWIDGETSIZE_MAX);
	m_saveAction->setEnabled(true);
	setWindowTitle("LCM - Combat active");
}


MainWindow::~MainWindow()
{
}
