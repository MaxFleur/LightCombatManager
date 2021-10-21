#include "../../include/GUI/MainWindow.hpp"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
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
	m_newCombatAction = new QAction(tr("&New Combat"), this);
	m_newCombatAction->setShortcuts(QKeySequence::New);
	m_newCombatAction->setStatusTip(tr("Start a new Combat."));
	connect(m_newCombatAction, &QAction::triggered, this, &MainWindow::newCombat);

	m_saveAction = new QAction(tr("&Save Table"), this);
	m_saveAction->setShortcuts(QKeySequence::Save);
	m_saveAction->setStatusTip(tr("Save the created Table."));
	connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveTable);

	m_openAction = new QAction(tr("&Open Table"), this);
	m_openAction->setShortcuts(QKeySequence::Open);
	m_openAction->setStatusTip(tr("Open an existing Table."));
	connect(m_openAction, &QAction::triggered, this, &MainWindow::openTable);

	m_aboutAction = new QAction(tr("&About"), this);
	m_aboutAction->setStatusTip(tr("About Light Combat Manager"));
	connect(m_aboutAction, &QAction::triggered, this, &MainWindow::about);

	m_aboutQtAction = new QAction(tr("About &Qt"), this);
	m_aboutQtAction->setStatusTip(tr("About QT Version and License"));
	connect(m_aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

	// Menus
	auto *const fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(m_newCombatAction);
	fileMenu->addAction(m_saveAction);
	fileMenu->addAction(m_openAction);
	fileMenu->addSeparator();

	auto *const helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(m_aboutAction);
	helpMenu->addAction(m_aboutQtAction);

	m_char = std::make_shared<CharacterHandler>();
	m_file = std::make_shared<FileHandler>();

	setWelcomingWidget();
}


void
MainWindow::newCombat()
{
	// If a creation is currently running, asks if a new Combat should be started anyway
	if (m_isCreationActive) {
		auto const reply = QMessageBox::question(
			this,
			tr("Start a new Combat?"),
			tr(
				"Currently, you are storing Characters for a beginning Combat. Do you want to create a new Combat anyway? "
				"This will delete all Characters you have stored for this Combat."),
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			m_char->clearCharacters();
		} else {
			return;
		}
		// If the Table is active right now, asks if the current Table should be saved
	} else if (m_isTableActive) {
		auto const reply = QMessageBox::question(
			this,
			tr("Start a new Combat?"),
			tr("Do you want to save the current Combat before starting a new one?"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes) {
			auto const code = saveTable();
			if (!code) {
				return;
			}
		} else if (reply == QMessageBox::Cancel) {
			return;
		}
		m_isTableActive = false;
		m_char->clearCharacters();
	}
	m_isCreationActive = true;
	setCharacterCreationWidget();
}


bool
MainWindow::saveTable()
{
	if (m_tableWidget->getRowCount() < 2) {
		auto const reply = QMessageBox::critical(
			this,
			tr("Too few Table entries!"),
			tr("The Table contains less then two entries."));
		return false;
	}
	auto const filename = QFileDialog::getSaveFileName(
		this,
		tr("Save Table"),
		QDir::currentPath(),
		tr("Table (*.csv);;All Files (*)"));

	if (filename.isEmpty()) {
		// No file provided or Cancel pressed
		return false;
	}

	m_file->saveTable(
		m_tableWidget->getTableWidget(),
		filename,
		m_tableWidget->getRowEntered(),
		m_tableWidget->getRoundCounter());
	// Success
	return true;
}


void
MainWindow::openTable()
{
	// If a creation is currently running, asks if a Table shoud be opened anyway
	if (m_isCreationActive) {
		auto const reply = QMessageBox::question(
			this,
			tr("Open Table?"),
			tr(
				"Currently, you are storing Characters for a beginning Combat. Do you want to open another Table anyway? "
				"This will delete all Characters you have stored for this Combat."),
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			m_char->clearCharacters();
		} else {
			return;
		}
		// If the Table is active right now, asks if the current Table should be saved
	} else if (m_isTableActive) {
		auto const reply = QMessageBox::question(
			this,
			tr("Save current Table?"),
			tr("Do you want to save the current Combat before opening another Table?"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes) {
			auto const code = saveTable();
			if (!code) {
				return;
			}
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
		// Table and char creation not active for a short time
		m_isCreationActive = false;
		m_isTableActive = false;
		setTableWidget(true, m_file->getData());
		break;
	}
	case 1:
	{
		auto const reply = QMessageBox::critical(
			this,
			tr("Wrong Table format!"),
			tr("The loading of the Table failed because the Table has the wrong format."));
		break;
	}
	case 2:
		break;
	}
}


void
MainWindow::about()
{
	QMessageBox::about(
		this,
		tr("About Light Combat Manager"),
		tr(
			"<p>Light Combat Manager. A simple Combat Manager for Pathfinder 1e. <br>"
			"<a href='https://github.com/MaxFleur/LightCombatManager'>Code available on Github.</a></p>"
			"<p>Version 1.2.0 Beta.</p>"));
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
		tr("Exit Combat?"),
		tr("Are you sure you want to exit the Combat? All created Characters will be lost."),
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
	// Ignore if no Character is stored yet
	if (m_char->getCharacters().size() > 0) {
		auto const reply = QMessageBox::question(
			this,
			tr("Cancel creation?"),
			tr(
				"Are you sure you want to cancel the Character creation? All created Characters will be lost."),
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
	// At least two Characters must be stored. Alternative: 1 Character stored and a name has been entered.
	if ((m_char->getCharacters().size() == 1 &&
	     m_characterCreationWidget->isNameEmpty()) || m_char->getCharacters().empty()) {
		auto const reply = QMessageBox::warning(
			this,
			tr("Could not finish!"),
			tr("Please store at least two Characters before creating the Combat Table!"));
		return;
	}
	// Display a question message asking if the creation should be finished
	auto const reply = QMessageBox::question(
		this,
		tr("Finish creation?"),
		tr("Are you sure you want to finish the Character creation?"),
		QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		m_characterCreationWidget->storeLastCharacter();
		m_isCreationActive = false;
		setTableWidget(false);
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
MainWindow::setCharacterCreationWidget()
{
	m_characterCreationWidget = new CharacterCreationWidget(m_char, this);
	setCentralWidget(m_characterCreationWidget);
	setFixedSize(700, 280);
	m_characterCreationWidget->setFocus();
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
	setWindowTitle(tr("LCM - Creating new Combat"));
}


void
MainWindow::setTableWidget(bool isDataStored, QString data)
{
	m_tableWidget = new TableWidget(m_char, isDataStored, data, this);
	setCentralWidget(m_tableWidget);
	connect(m_tableWidget, &TableWidget::exit, this, &MainWindow::exitCombat);
	connect(
		m_tableWidget,
		&TableWidget::tableSet,
		this,
		[this] (int height) {
			setMinimumSize(710, height);
			setMaximumWidth(QWIDGETSIZE_MAX);
		});
	m_isTableActive = true;
	setMinimumSize(710, m_tableWidget->getHeight());
	setMaximumWidth(QWIDGETSIZE_MAX);
	m_saveAction->setEnabled(true);
	setWindowTitle(tr("LCM - Combat Active"));
}


void
MainWindow::closeEvent(QCloseEvent *event)
{
	// If the creation is active, ask if it should be closed
	if (m_isCreationActive) {
		auto const reply = QMessageBox::question(
			this,
			tr("Exit"),
			tr(
				"You are in the Character Creation right now! Do you want to exit the program anyway? All stored Characters will be lost."),
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			QApplication::exit;
		} else {
			event->ignore();
		}
	}
	// If the Table is active, send a question if the Table should be saved
	if (m_isTableActive) {
		auto const reply = QMessageBox::question(
			this,
			tr("Exit"),
			tr(
				"Currently, you are in a Combat. Do you want to save the Characters before exiting the program?"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes) {
			auto const code = saveTable();
			if (!code) {
				event->ignore();
			}
		} else if (reply == QMessageBox::Cancel) {
			event->ignore();
		}
		QApplication::exit;
	}
}


MainWindow::~MainWindow()
{
}
