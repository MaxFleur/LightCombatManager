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
#include <QTimer>

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
	connect(saveAction, &QAction::triggered, this, &MainWindow::saveCombat);
	connect(this, &MainWindow::setSaveAction, this, [saveAction] (bool enable) {
		saveAction->setEnabled(enable);
	});

	auto *const openAction = new QAction(tr("&Open Table"), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an existing Table."));
	connect(openAction, &QAction::triggered, this, &MainWindow::loadCombat);

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

	handleSubDir();
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
		if (!m_tableWidget->isEmpty()) {
			const auto newCombatMessage = m_changeOccured ?
						      tr("Do you want to save the current Combat before starting a new one?") :
						      tr("Do you want to start a new Combat?");

			auto *const msgBox = new QMessageBox(this);
			msgBox->setStandardButtons(
				m_changeOccured ?
				QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel :
				QMessageBox::Yes | QMessageBox::No);
			msgBox->setText(newCombatMessage);

			const auto val = msgBox->exec();
			switch (val) {
			case QMessageBox::Save:
			{
				if (!saveCombat()) {
					return;
				}
				break;
			}
			case QMessageBox::Discard:
			case QMessageBox::Yes:
				break;
			case QMessageBox::Cancel:
			case QMessageBox::No:
				return;
			}
		}
	}
	m_tableInFile = false;
	setTableWidget(false, true);
}


bool
MainWindow::saveCombat()
{
	if (!m_changeOccured) {
		return false;
	}

	if (m_tableWidget->isEmpty()) {
		auto const reply = QMessageBox::critical(
			this,
			tr("Table empty!"),
			tr("Can't save an empty table."));
		return false;
	}
	if (Utils::containsSemicolon(m_tableWidget->getTableWidget())) {
		auto const reply = QMessageBox::critical(
			this,
			tr("Semicolons detected!"),
			tr("Can't save because the table contains semicolons. Please remove them and continue."));
		return false;
	}

	QString fileName;
	// Save to standard save dir if a new combat has been started
	if (!m_tableInFile) {
		fileName = QFileDialog::getSaveFileName(this, tr("Save Table"), m_saveDir,
							tr("Table (*.csv);;All Files (*)"));

		if (fileName.isEmpty()) {
			// No file provided or Cancel pressed
			return false;
		}
		// Otherwise, just overwrite the loaded file
	} else {
		fileName = m_openDir;
	}

	auto const code = m_file->saveTable(
		m_tableWidget->getTableWidget(),
		fileName,
		m_tableWidget->getRowEntered(),
		m_tableWidget->getRoundCounter());
	if (code) {
		m_changeOccured = false;
		m_tableInFile = true;
		writeSettings(fileName, false);
		setCombatTitle();
		// Success
		return true;
	}
	auto const reply = QMessageBox::critical(
		this,
		tr("Could not save table!"),
		tr("Failed to write file."));
	return false;
}


void
MainWindow::loadCombat()
{
	// Check if a table is active right now
	if (m_isTableActive) {
		if (!m_tableWidget->isEmpty()) {
			const auto openCombatMessage = m_changeOccured ?
						       tr("Do you want to save the current Combat before opening another existing Combat?") :
						       tr("Do you want to open another existing Combat?");
			auto *const msgBox = new QMessageBox(this);
			msgBox->setStandardButtons(
				m_changeOccured ?
				QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel :
				QMessageBox::Yes | QMessageBox::No);
			msgBox->setText(openCombatMessage);

			const auto val = msgBox->exec();
			switch (val) {
			case QMessageBox::Save:
			{
				if (!saveCombat()) {
					return;
				}
				break;
			}
			case QMessageBox::Discard:
			case QMessageBox::Yes:
				break;
			case QMessageBox::Cancel:
			case QMessageBox::No:
				return;
			}
		}
	}
	auto const fileName =
		QFileDialog::getOpenFileName(this, "Open Table", m_openDir, ("csv File(*.csv)"));
	auto const code = m_file->getCSVData(fileName);

	switch (code) {
	case 0:
	{
		// Table not active for a short time
		m_isTableActive = false;
		m_tableInFile = true;
		// Save the opened file dir
		writeSettings(fileName, false);
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
		   "<p>Version 1.5.0. <br>"
		   "<a href='https://github.com/MaxFleur/LightCombatManager/releases'>Changelog</a></p>"));
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
	setWindowTitle("LCM");

	m_welcomeWidget = new WelcomeWidget(this);
	setMinimumSize(0, 0);
	setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
	setCentralWidget(m_welcomeWidget);

	m_tableInFile = false;
	emit setSaveAction(false);
}


void
MainWindow::setTableWidget(bool isDataStored, bool newCombatStarted, QString data)
{
	m_tableWidget = new TableWidget(isDataStored, data, this);
	setCentralWidget(m_tableWidget);
	connect(m_tableWidget, &TableWidget::exit, this, &MainWindow::exitCombat);
	connect(m_tableWidget, &TableWidget::tableHeightSet, this, [this] (int height) {
		if (height > START_HEIGHT) {
			setFixedHeight(height);
		}
	});
	connect(m_tableWidget, &TableWidget::tableWidthSet, this, [this] (int width) {
		if (width > this->width()) {
			// @note A single immediate call to resize() won't actually resize the window
			// So the function is called with a minimal delay of 1 ms, which will actually
			// resize the main window
			QTimer::singleShot(1, [this, width]() {
				resize(width, this->height());
			});
		}
	});
	connect(m_tableWidget, &TableWidget::changeOccured, this, [this] () {
		m_changeOccured = true;
		// Mark change in window
		setCombatTitle();
	});
	connect(m_tableWidget, &TableWidget::characterNotSelected, this, [this] () {
		auto const reply = QMessageBox::warning(
			this,
			tr("Could not remove Character!"),
			tr("Please select a Character with the Mouse Key before deleting!"));
	});

	if (!newCombatStarted) {
		m_tableWidget->generateTable();
		const auto height = m_tableWidget->getHeight();
		height > START_HEIGHT ?  setFixedHeight(height) : setFixedHeight(START_HEIGHT);

		m_changeOccured = false;
		setCombatTitle();
	} else {
		setCombatTitle();
		setFixedHeight(START_HEIGHT);
		m_tableWidget->openAddCharacterDialog();
	}

	m_isTableActive = true;
	emit setSaveAction(true);
}


void
MainWindow::setCombatTitle()
{
	m_changeOccured ? setWindowTitle(tr("LCM - Combat Active *")) : setWindowTitle(tr("LCM - Combat Active"));
}


void
MainWindow::writeSettings(QString fileName, bool setSaveDir)
{
	m_openDir = fileName;

	QSettings settings;
	settings.setValue("dir_open", fileName);
	// Only set the standard save dir at the first program start, after that it's saved
	if (setSaveDir) {
		settings.setValue("dir_save", fileName);
	}
}


void
MainWindow::readSettings()
{
	QSettings settings;

	m_saveDir = settings.value("dir_save").isValid() ? settings.value("dir_save").toString() : QString();
	m_openDir = settings.value("dir_open").isValid() ? settings.value("dir_open").toString() : QString();
}


void
MainWindow::closeEvent(QCloseEvent *event)
{
	// Check if a table is active and filled
	if (m_isTableActive && !m_tableWidget->isEmpty()) {
		const auto closeCombatMessage = m_changeOccured ?
						tr("Currently, you are in a Combat. Do you want to save the Characters before exiting the program?") :
						tr("Do you really want to exit the application?");

		auto *const msgBox = new QMessageBox(this);
		msgBox->setStandardButtons(
			m_changeOccured ?
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel :
			QMessageBox::Yes | QMessageBox::No);
		msgBox->setText(closeCombatMessage);

		const auto val = msgBox->exec();
		switch (val) {
		case QMessageBox::Save:
		{
			saveCombat() ? event->accept() : event->ignore();
			break;
		}
		case QMessageBox::Discard:
			event->accept();
			break;
		case QMessageBox::Yes:
			return;
		case QMessageBox::Cancel:
		case QMessageBox::No:
			event->ignore();
			break;
		}
	}
}


void
MainWindow::handleSubDir()
{
	// Create a subdir to save the tables into
	QDir dir(QDir::currentPath());
	// Write into settings so this subdir is used as standard path for saving tables
	if (dir.mkdir("tables")) {
		const auto tableSubDir = QDir::currentPath() + "/tables";
		// Save and open get same directory, user might change it later
		m_saveDir = tableSubDir;
		m_openDir = tableSubDir;

		writeSettings(tableSubDir, true);
		return;
	}
}
