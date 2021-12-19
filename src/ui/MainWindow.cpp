#include "MainWindow.hpp"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QKeySequence>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QString>

#include "table/TableWidget.hpp"
#include "Utils.hpp"
#include "WelcomeWidget.hpp"

MainWindow::MainWindow()
{
	// Actions
	auto *const newCombatAction = new QAction(tr("&New Combat"), this);
	newCombatAction->setShortcuts(QKeySequence::New);
	newCombatAction->setStatusTip(tr("Start a new Combat."));
	connect(newCombatAction, &QAction::triggered, this, &MainWindow::newCombat);

	auto *const saveAction = new QAction(tr("&Save Table"), this);
	saveAction->setShortcuts(QKeySequence::Save);
	saveAction->setStatusTip(tr("Save the created Table."));
	connect(saveAction, &QAction::triggered, this, &MainWindow::saveTable);
	connect(this, &MainWindow::setSaveAction, this, [saveAction] (bool enable) {
		saveAction->setEnabled(enable);
	});

	auto *const openAction = new QAction(tr("&Open Table"), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an existing Table."));
	connect(openAction, &QAction::triggered, this, &MainWindow::openTable);

	auto *const aboutAction = new QAction(tr("&About"), this);
	aboutAction->setStatusTip(tr("About Light Combat Manager"));
	connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

	auto *const aboutQtAction = new QAction(tr("About &Qt"), this);
	aboutQtAction->setStatusTip(tr("About QT Version and License"));
	connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

	// Menus
	auto *const fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newCombatAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(openAction);
	fileMenu->addSeparator();

	auto *const helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);

	readSettings();

	m_file = std::make_shared<FileHandler>();

	resize(START_WIDTH, START_HEIGHT);
	setWelcomingWidget();
}


void
MainWindow::newCombat()
{
	// Check if a table is active
	if (m_isTableActive) {
		// Check if it's not empty and if the user has saved before hitting the shortcut
		if (!m_tableWidget->isEmpty() && m_changeOccured) {
			auto const reply = QMessageBox::question(
				this,
				tr("Start a new Combat?"),
				tr("Do you want to save the current Combat before starting a new one?"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			if (reply == QMessageBox::Yes) {
				auto const code = saveTable();
				if (code != 0) {
					return;
				}
			} else if (reply == QMessageBox::Cancel) {
				return;
			}
		}
	}
	setTableWidget(false, true);
}


int
MainWindow::saveTable()
{
	if (m_tableWidget->isEmpty()) {
		auto const reply = QMessageBox::critical(
			this,
			tr("Table empty!"),
			tr("Can't save an empty table."));
		return 1;
	}
	if (Utils::containsSemicolon(m_tableWidget->getTableWidget())) {
		auto const reply = QMessageBox::critical(
			this,
			tr("Semicolons detected!"),
			tr("Can't save because the table contains semicolons. Please remove them and continue."));
		return 2;
	}

	auto const fileName = QFileDialog::getSaveFileName(this, tr("Save Table"), m_currentDir,
							   tr("Table (*.csv);;All Files (*)"));

	if (fileName.isEmpty()) {
		// No file provided or Cancel pressed
		return 3;
	}

	auto const code = m_file->saveTable(
		m_tableWidget->getTableWidget(),
		fileName,
		m_tableWidget->getRowEntered(),
		m_tableWidget->getRoundCounter());
	if (code) {
		writeSettings(fileName);
		m_changeOccured = false;

		// Success
		return 0;
	}
	auto const reply = QMessageBox::critical(
		this,
		tr("Could not save table!"),
		tr("Failed to write file."));
	return 4;
}


void
MainWindow::openTable()
{
	// Check if a table is active right now
	if (m_isTableActive) {
		// Check if the table is filled and if the user has not saved already
		if (!m_tableWidget->isEmpty() && m_changeOccured) {
			auto const reply = QMessageBox::question(
				this,
				tr("Save current Table?"),
				tr("Do you want to save the current Combat before opening another Table?"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			if (reply == QMessageBox::Yes) {
				auto const code = saveTable();
				if (code != 0) {
					return;
				}
			} else if (reply == QMessageBox::Cancel) {
				return;
			}
		}
	}
	auto const fileName =
		QFileDialog::getOpenFileName(this, "Open Table", m_currentDir, ("csv File(*.csv)"));
	auto const code = m_file->getCSVData(fileName);

	switch (code) {
	case 0:
	{
		// Table not active for a short time
		m_isTableActive = false;
		writeSettings(fileName);
		setTableWidget(true, false, m_file->getData());
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
		tr("<p>Light Combat Manager. A simple Combat Manager for Pathfinder 1e.<br>"
		   "<a href='https://github.com/MaxFleur/LightCombatManager'>Code available on Github.</a></p>"
		   "<p>Version 1.4.4.</p>"));
}


void
MainWindow::exitCombat()
{
	auto const reply = QMessageBox::question(
		this,
		tr("Return?"),
		tr("Are you sure you want to return to the Main Window? This will end the current Combat."),
		QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes) {
		setWelcomingWidget();
		m_isTableActive = false;
	}
}


void
MainWindow::setWelcomingWidget()
{
	m_welcomeWidget = new WelcomeWidget(this);
	setMinimumSize(0, 0);
	setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
	setCentralWidget(m_welcomeWidget);
	emit setSaveAction(false);
	setWindowTitle("LCM");
}


void
MainWindow::setTableWidget(bool isDataStored, bool newCombatStarted, QString data)
{
	m_tableWidget = new TableWidget(isDataStored, newCombatStarted, data, this);
	setCentralWidget(m_tableWidget);
	connect(m_tableWidget, &TableWidget::exit, this, &MainWindow::exitCombat);
	connect(m_tableWidget, &TableWidget::tableSet, this, [this] (int height) {
		if (height > START_HEIGHT) {
			setFixedHeight(height);
		}
	});
	connect(m_tableWidget, &TableWidget::changeOccured, this, [this] () {
		m_changeOccured = true;
	});
	m_isTableActive = true;
	m_changeOccured = false;

	auto height = isDataStored ? m_tableWidget->getHeight() : START_HEIGHT;
	setFixedHeight(height);

	emit setSaveAction(true);
	setWindowTitle(tr("LCM - Combat Active"));

	if (newCombatStarted) {
		m_tableWidget->openAddCharacterDialog();
	}
}


void
MainWindow::writeSettings(QString fileName)
{
	m_currentDir = fileName;

	QSettings settings;
	settings.setValue("Dir", fileName);
}


void
MainWindow::readSettings()
{
	QSettings settings;
	m_currentDir = settings.value("Dir").isValid() ? settings.value("Dir").toString() : "";
}


void
MainWindow::closeEvent(QCloseEvent *event)
{
	// Check if a table is active and filled
	if (m_isTableActive && !m_tableWidget->isEmpty()) {
		// Just continue if the user has not done anything after saving
		if (m_changeOccured) {
			auto const reply = QMessageBox::question(
				this,
				tr("Exit"),
				tr("Currently, you are in a Combat. "
				   "Do you want to save the Characters before exiting the program?"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

			if (reply == QMessageBox::Yes) {
				auto const code = saveTable();
				if (code != 0) {
					event->ignore();
				} else {
					event->accept();
				}
			} else if (reply == QMessageBox::No) {
				event->accept();
			} else if (reply == QMessageBox::Cancel) {
				event->ignore();
			}
		} else {
			auto const reply = QMessageBox::question(
				this,
				tr("Exit"),
				tr("Do you really want to exit the application?"),
				QMessageBox::Yes | QMessageBox::No);

			if (reply == QMessageBox::Yes) {
				return;
			} else {
				event->ignore();
			}
		}
	}
}
