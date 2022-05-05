#include "MainWindow.hpp"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QKeySequence>
#include <QMenuBar>
#include <QMessageBox>
#include <QString>
#include <QTimer>

#include "DirSettings.hpp"
#include "RuleSettings.hpp"
#include "SettingsDialog.hpp"
#include "TableWidget.hpp"
#include "Utils.hpp"
#include "WelcomeWidget.hpp"

MainWindow::MainWindow()
{
	// Actions
	auto *const newCombatAction = new QAction(style()->standardIcon(QStyle::SP_FileIcon), tr("&New Combat"), this);
	newCombatAction->setShortcuts(QKeySequence::New);
	connect(newCombatAction, &QAction::triggered, this, &MainWindow::newCombat);

	auto *const openTableAction = new QAction(style()->standardIcon(QStyle::SP_DirOpenIcon), tr("&Open..."), this);
	openTableAction->setShortcuts(QKeySequence::Open);
	connect(openTableAction, &QAction::triggered, this, &MainWindow::openTable);

	auto *const closeTableAction = new QAction(style()->standardIcon(QStyle::SP_TitleBarCloseButton), tr("&Close"), this);
	closeTableAction->setShortcuts(QKeySequence::Close);
	connect(closeTableAction, &QAction::triggered, this, [this] () {
		if (m_isTableActive) {
			exitCombat();
		} else {
			QApplication::quit();
		}
	});

	auto *const saveTableAction = new QAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&Save"), this);
	saveTableAction->setShortcuts(QKeySequence::Save);
	connect(saveTableAction, &QAction::triggered, this, &MainWindow::saveTable);

	auto *const saveAsAction = new QAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&Save As..."), this);
	saveAsAction->setShortcuts(QKeySequence::SaveAs);
	connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);
	// Both options have to be enabled or disabled simultaneously
	connect(this, &MainWindow::setSaveAction, this, [saveTableAction, saveAsAction] (bool enable) {
		saveTableAction->setEnabled(enable);
		saveAsAction->setEnabled(enable);
	});

	auto *const openSettingsAction = new QAction(tr("Settings..."), this);
	connect(openSettingsAction, &QAction::triggered, this, &MainWindow::openSettings);

	auto *const aboutAction = new QAction(style()->standardIcon(QStyle::SP_DialogHelpButton), tr("&About"), this);
	connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

	auto *const aboutQtAction = new QAction(style()->standardIcon(QStyle::SP_TitleBarMenuButton), tr("About &Qt"), this);
	connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

	auto *const fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newCombatAction);
	fileMenu->addAction(openTableAction);
	fileMenu->addAction(closeTableAction);
	fileMenu->addAction(saveTableAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addAction(openSettingsAction);
	fileMenu->addSeparator();

	auto *const helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);

	m_file = std::make_shared<FileHandler>();
	m_ruleSettings = std::make_shared<RuleSettings>();
	m_dirSettings = std::make_shared<DirSettings>();

	resize(START_WIDTH, START_HEIGHT);
	setWelcomingWidget();
}


void
MainWindow::newCombat()
{
	// Check if a table is active
	if (m_isTableActive && !m_tableWidget->isEmpty()) {
		if (createSaveMessageBox(isWindowModified() ?
					 tr("Do you want to save the current Combat before starting a new one?") :
					 tr("Do you want to start a new Combat?"), false) == 0) {
			return;
		}
	}
	m_tableInFile = false;

	m_fileName = QString();
	setCombatTitle(false, true);

	setTableWidget(false, true);
}


bool
MainWindow::saveTable()
{
	if (!isWindowModified()) {
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
		fileName = QFileDialog::getSaveFileName(this, tr("Save Table"), m_dirSettings->saveDir,
							tr("Table (*.csv);;All Files (*)"));

		if (fileName.isEmpty()) {
			// No file provided or Cancel pressed
			return false;
		}
		// Otherwise, just overwrite the loaded file
	} else {
		fileName = m_dirSettings->openDir;
	}

	if (m_file->saveTable(
		    m_tableWidget->getTableWidget(),
		    fileName,
		    m_tableWidget->getRowEntered(),
		    m_tableWidget->getRoundCounter(),
		    m_ruleSettings->ruleset,
		    m_ruleSettings->rollAutomatical)) {
		m_tableInFile = true;
		m_dirSettings->write(fileName, false);
		m_fileName = Utils::getCSVName(fileName);

		setCombatTitle(false);
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
MainWindow::saveAs()
{
	// Save state
	const auto saveChangeOccured = isWindowModified();
	const auto saveTableInFile = m_tableInFile;

	// Change variables so saveTable calls the file dialog
	setWindowModified(true);
	m_tableInFile = false;
	// Restore old state if the save fails
	if (!saveTable()) {
		setWindowModified(saveChangeOccured);
		m_tableInFile = saveTableInFile;
	}
}


void
MainWindow::openTable()
{
	// Check if a table is active right now
	if (m_isTableActive && !m_tableWidget->isEmpty()) {
		if (createSaveMessageBox(isWindowModified() ?
					 tr("Do you want to save the current Combat before opening another existing Combat?") :
					 tr("Do you want to open another existing Combat?"), false) == 0) {
			return;
		}
	}
	auto const fileName =
		QFileDialog::getOpenFileName(this, "Open Table", m_dirSettings->openDir, ("csv File(*.csv)"));
	auto const code = m_file->getCSVData(fileName);

	auto rulesModified = false;

	switch (code) {
	case 0:
	{
		if (!checkStoredTableRules(m_file->getData())) {
			auto *const msgBox = createRuleChangeMessageBox();

			switch (msgBox->exec()) {
			case QMessageBox::Apply:
				m_ruleSettings->write(m_loadedTableRule, m_loadedTableRollAutomatically);
				break;
			case QMessageBox::Ignore:
				rulesModified = true;
				break;
			case QMessageBox::Cancel:
				return;
			}
		}
		// Table not active for a short time
		m_isTableActive = false;
		m_tableInFile = true;
		// Save the opened file dir
		m_dirSettings->write(fileName, false);
		m_fileName = Utils::getCSVName(fileName);
		setTableWidget(true, false, m_file->getData());

		// If the settings rules are applied to the table, it is modified
		setCombatTitle(rulesModified);
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
MainWindow::openSettings()
{
	auto *const dialog = new SettingsDialog(m_ruleSettings, m_isTableActive, this);
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->show();
}


void
MainWindow::about()
{
	QMessageBox::about(
		this,
		tr("About Light Combat Manager"),
		tr("<p>Light Combat Manager. A small and simple Combat Manager for D&D-like games.<br>"
		   "<a href='https://github.com/MaxFleur/LightCombatManager'>Code available on Github.</a></p>"
		   "<p>Version 1.7.0.<br>"
		   "<a href='https://github.com/MaxFleur/LightCombatManager/releases'>Changelog</a></p>"));
}


void
MainWindow::exitCombat()
{
	if (createSaveMessageBox(isWindowModified() ?
				 tr("Do you want to save the current Combat before exiting to the Main Window?") :
				 tr("Are you sure you want to return to the Main Window? This will end the current Combat."), false) == 0) {
		return;
	}
	setWelcomingWidget();
	m_isTableActive = false;
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
MainWindow::setTableWidget(bool isDataStored, bool newCombatStarted, const QString& data)
{
	m_tableWidget = new TableWidget(isDataStored, m_ruleSettings, data, this);
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
		setCombatTitle(true);
	});
	connect(m_tableWidget, &TableWidget::characterNotSelected, this, [this] () {
		auto const reply = QMessageBox::warning(
			this,
			tr("Could not remove Character!"),
			tr("Please select a Character with the Mouse Key before deleting!"));
	});

	setCombatTitle(false);

	if (!newCombatStarted) {
		m_tableWidget->generateTable();
		// Setting the table emits changeOccured because the cells are altered, so reset
		setCombatTitle(false);
		const auto height = m_tableWidget->getHeight();
		height > START_HEIGHT ? setFixedHeight(height) : setFixedHeight(START_HEIGHT);
	} else {
		setFixedHeight(START_HEIGHT);
		m_tableWidget->openAddCharacterDialog();
	}

	m_isTableActive = true;
	emit setSaveAction(true);
}


void
MainWindow::setCombatTitle(bool isCombatActive, bool newCombat)
{
	// Ensure a continuing for new combats
	if (isWindowModified() == isCombatActive && !newCombat) {
		return;
	}

	QString title = "LCM";
	m_fileName.isEmpty() ? title.append(" - " + (tr("Unnamed Combat"))) : title.append(" - " + m_fileName + "[*]");
	setWindowModified(isCombatActive);

	setWindowTitle(title);
}


int
MainWindow::createSaveMessageBox(const QString& tableMessage, bool isClosing)
{
	auto *const msgBox = new QMessageBox(this);
	msgBox->setIcon(QMessageBox::Question);
	msgBox->setStandardButtons(
		isWindowModified() ?
		QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel :
		QMessageBox::Yes | QMessageBox::No);
	msgBox->setText(tableMessage);
	if (isClosing) {
		isWindowModified() ? msgBox->setWindowTitle(tr("Save and exit?")) : msgBox->setWindowTitle(tr("Exit application?"));
	} else {
		msgBox->setWindowTitle(tr("Save Combat?"));
	}

	/* openTable and newCombat use an identical message box, so do the handling directly */
	if (!isClosing) {
		switch (msgBox->exec()) {
		case QMessageBox::Save:
		{
			if (!saveTable()) {
				return 0;
			}
			break;
		}
		case QMessageBox::Discard:
		case QMessageBox::Yes:
			break;
		case QMessageBox::Cancel:
		case QMessageBox::No:
			return 0;
		}
		return 1;
	}

	return msgBox->exec();
}


QMessageBox *
MainWindow::createRuleChangeMessageBox()
{
	const auto message = tr("The Table you are trying to load uses another ruleset than you have stored in your rule settings! <br><br>"
				"Your ruleset: <b>") + Utils::getRulesetName(m_ruleSettings->ruleset) + "</b>, " +
			     "<b>" + Utils::getAutoRollEnabled(m_ruleSettings->rollAutomatical) + "</b> <br>" +
			     "The stored table ruleset is: <b>" + Utils::getRulesetName(m_loadedTableRule) + "</b>, " +
			     "<b>" + Utils::getAutoRollEnabled(m_loadedTableRollAutomatically) + "</b> <br><br>" +
			     "Do you want to apply the stored Table ruleset to your settings or ignore it?";
	auto *const msgBox = new QMessageBox(
		QMessageBox::Warning,
		tr("Different rulesets detected!"),
		message,
		QMessageBox::Apply | QMessageBox::Ignore | QMessageBox::Cancel);

	msgBox->setButtonText(QMessageBox::Apply, tr("Apply Table ruleset to Settings"));
	msgBox->setButtonText(QMessageBox::Ignore, tr("Ignore Table ruleset"));

	return msgBox;
}


void
MainWindow::closeEvent(QCloseEvent *event)
{
	// Check if a table is active and filled
	if (m_isTableActive && !m_tableWidget->isEmpty()) {
		const auto val = createSaveMessageBox(isWindowModified() ?
						      tr("Currently, you are in a Combat. Do you want to save the Characters before exiting the program?") :
						      tr("Do you really want to exit the application?"), true);

		switch (val) {
		case QMessageBox::Save:
		{
			saveTable() ? event->accept() : event->ignore();
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


bool
MainWindow::checkStoredTableRules(QString data)
{
	// Get the first row of the stored table
	const auto firstRowData = data.split("\n").at(1).split(";");
	// Get the loaded ruleset and roll automatically variable
	m_loadedTableRule = static_cast<RuleSettings::Ruleset>(firstRowData[COL_RULESET].toInt());
	m_loadedTableRollAutomatically = static_cast<bool>(firstRowData[COL_ROLL_AUTOMATICALLY].toInt());

	return m_ruleSettings->ruleset == m_loadedTableRule &&
	       m_ruleSettings->rollAutomatical == m_loadedTableRollAutomatically;
}
