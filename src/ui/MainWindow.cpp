#include "MainWindow.hpp"

#include "CombatWidget.hpp"
#include "SettingsDialog.hpp"
#include "UtilsGeneral.hpp"
#include "WelcomeWidget.hpp"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QKeySequence>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>

MainWindow::MainWindow()
{
    // Actions
    m_newCombatAction = new QAction(tr("&New"), this);
    m_newCombatAction->setShortcuts(QKeySequence::New);
    connect(m_newCombatAction, &QAction::triggered, this, &MainWindow::newCombat);

    m_openCombatAction = new QAction(tr("&Open..."), this);
    m_openCombatAction->setShortcuts(QKeySequence::Open);
    connect(m_openCombatAction, &QAction::triggered, this, &MainWindow::openTable);

    m_saveAction = new QAction(tr("&Save"), this);
    m_saveAction->setShortcuts(QKeySequence::Save);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveTable);

    m_saveAsAction = new QAction(tr("&Save As..."), this);
    m_saveAsAction->setShortcuts(QKeySequence::SaveAs);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);
    // Both options have to be enabled or disabled simultaneously
    connect(this, &MainWindow::setSaveAction, this, [this] (bool enable) {
        m_saveAction->setEnabled(enable);
        m_saveAsAction->setEnabled(enable);
    });

    auto* const closeAction = new QAction(QIcon(":/icons/menus/close.svg"), tr("&Close"), this);
    closeAction->setShortcuts(QKeySequence::Close);
    connect(closeAction, &QAction::triggered, this, [this] () {
        m_isTableActive ? exitCombat() : QApplication::quit();
    });

    m_openSettingsAction = new QAction(tr("Settings..."), this);
    connect(m_openSettingsAction, &QAction::triggered, this, &MainWindow::openSettings);

    m_aboutLCMAction = new QAction(tr("&About LCM"), this);
    connect(m_aboutLCMAction, &QAction::triggered, this, &MainWindow::about);

    auto *const aboutQtAction = new QAction(style()->standardIcon(QStyle::SP_TitleBarMenuButton), tr("About &Qt"), this);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

    // Add actions
    auto *const fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_newCombatAction);
    fileMenu->addAction(m_openCombatAction);
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_saveAsAction);
    fileMenu->addAction(closeAction);
    fileMenu->addAction(m_openSettingsAction);
    fileMenu->addSeparator();

    auto *const helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(m_aboutLCMAction);
    helpMenu->addAction(aboutQtAction);

    m_fileHandler = std::make_unique<FileHandler>();

    setMainWindowIcons();
    resize(START_WIDTH, START_HEIGHT);
    setWelcomingWidget();
}


void
MainWindow::newCombat()
{
    // Check if a table is active
    if (m_isTableActive &&
        createSaveMessageBox(isWindowModified() ? tr("Do you want to save the current Combat before starting a new one?")
                                                : tr("Do you want to start a new Combat?"), false) == 0) {
        return;
    }
    m_isTableSavedInFile = false;

    m_fileName = QString();

    setTableWidget(false, true);
    setCombatTitle(true);
}


bool
MainWindow::saveTable()
{
    if (!isWindowModified()) {
        return false;
    }

    QString fileName;
    // Save to standard save dir if a new combat has been started
    if (!m_isTableSavedInFile) {
        fileName = QFileDialog::getSaveFileName(this, tr("Save Table"), m_dirSettings.saveDir,
                                                tr("Table (*.lcm);;All Files (*)"));

        if (fileName.isEmpty()) {
            // No file provided or Cancel pressed
            return false;
        }
    } else {
        // Otherwise, just overwrite the loaded file
        fileName = m_dirSettings.openDir;
    }
    // Save the table
    auto* const combatTableWidget = m_combatWidget->getCombatTableWidget();
    const auto tableData = combatTableWidget->tableDataFromWidget();
    if (m_fileHandler->writeTableToFile(tableData, fileName, m_combatWidget->getRowEntered(),
                                        m_combatWidget->getRoundCounter(),
                                        m_ruleSettings.ruleset, m_ruleSettings.rollAutomatical)) {
        m_isTableSavedInFile = true;
        m_dirSettings.write(fileName, true);
        m_fileName = Utils::General::getCSVName(fileName);

        setCombatTitle(false);
        // Success
        return true;
    }
    QMessageBox::critical(this, tr("Could not save Table!"), tr("Failed to write file."));
    return false;
}


// Reopen the file dialog, even if the table has been saved already
void
MainWindow::saveAs()
{
    // Save state
    const auto saveChangeOccured = isWindowModified();
    const auto saveTableInFile = m_isTableSavedInFile;

    // Change variables to call the file dialog
    setWindowModified(true);
    m_isTableSavedInFile = false;
    // Restore old state if the save fails
    if (!saveTable()) {
        setWindowModified(saveChangeOccured);
        m_isTableSavedInFile = saveTableInFile;
    }
}


void
MainWindow::openTable()
{
    const auto fileName = QFileDialog::getOpenFileName(this, "Open Table", m_dirSettings.openDir, ("lcm File(*.lcm)"));
    // Return for equal names, but let it load the first time
    if (fileName == m_dirSettings.openDir && m_isTableAlreadyLoaded) {
        return;
    }
    // Check if a table is active right now
    if (m_isTableActive && isWindowModified() &&
        createSaveMessageBox(tr("Do you want to save the current Combat before opening another existing Combat?"), false) == 0) {
        return;
    }

    m_isTableAlreadyLoaded = true;
    auto rulesModified = false;

    switch (const auto code = m_fileHandler->getLCMStatus(fileName); code) {
    case 0:
    {
        if (!checkStoredTableRules(m_fileHandler->getData())) {
            const auto messageString = createRuleChangeMessageBoxText();
            auto *const msgBox = new QMessageBox(QMessageBox::Warning, tr("Different Rulesets detected!"), messageString, QMessageBox::Cancel);
            auto* const applyButton = msgBox->addButton(tr("Apply Table Ruleset to Settings"), QMessageBox::ApplyRole);
            auto* const ignoreButton = msgBox->addButton(tr("Ignore stored Table Ruleset"), QMessageBox::AcceptRole);

            msgBox->exec();
            if (msgBox->clickedButton() == applyButton) {
                m_ruleSettings.write(m_loadedTableRule, m_loadedTableRollAutomatically);
            } else if (msgBox->clickedButton() == ignoreButton) {
                rulesModified = true;
            } else {
                return;
            }
        }
        // Table not active for a short time
        m_isTableActive = false;
        m_isTableSavedInFile = true;
        // Save the opened file dir
        m_dirSettings.write(fileName);
        m_fileName = Utils::General::getCSVName(fileName);
        setTableWidget(true, false, m_fileHandler->getData());

        // If the settings rules are applied to the table, it is modified
        setCombatTitle(rulesModified);
        break;
    }
    case 1:
    {
        QMessageBox::critical(this, tr("Wrong Table format!"),
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
    auto *const dialog = new SettingsDialog(m_additionalSettings, m_ruleSettings, m_isTableActive, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setModal(true);
    dialog->show();
}


void
MainWindow::about()
{
    QMessageBox::about(this, tr("About Light Combat Manager"),
                       tr("<p>Light Combat Manager. A small, lightweight Combat Manager for d20-based role playing games.<br>"
                          "<a href='https://github.com/MaxFleur/LightCombatManager'>Code available on Github.</a></p>"
                          "<p>Version 2.0.0.<br>"
                          "<a href='https://github.com/MaxFleur/LightCombatManager/releases'>Changelog</a></p>"));
}


void
MainWindow::exitCombat()
{
    if (createSaveMessageBox(isWindowModified() ? tr("Do you want to save the current Combat before exiting to the Main Window?")
                                                : tr("Are you sure you want to return to the Main Window? This will end the current Combat."),
                             false) == 0) {
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

    m_isTableSavedInFile = false;
    emit setSaveAction(false);
}


void
MainWindow::setTableWidget(bool isDataStored, bool newCombatStarted, const QJsonObject& jsonObjectData)
{
    m_combatWidget = new CombatWidget(m_additionalSettings, m_ruleSettings, width(), isDataStored, jsonObjectData, this);
    setCentralWidget(m_combatWidget);
    connect(m_combatWidget, &CombatWidget::exit, this, &MainWindow::exitCombat);
    connect(m_combatWidget, &CombatWidget::tableHeightSet, this, [this] (unsigned int height) {
        if (height > START_HEIGHT) {
            setFixedHeight(height);
        }
    });
    connect(m_combatWidget, &CombatWidget::tableWidthSet, this, [this] (int tableWidth) {
        if (tableWidth > width()) {
            // @note A single immediate call to resize() won't actually resize the window
            // So the function is called with a minimal delay of 1 ms, which will actually
            // resize the main window
            QTimer::singleShot(1, [this, tableWidth]() {
                resize(tableWidth, height());
            });
        }
    });
    connect(m_combatWidget, &CombatWidget::changeOccured, this, [this] {
        setCombatTitle(true);
    });

    setCombatTitle(false);

    if (newCombatStarted) {
        setFixedHeight(START_HEIGHT);
        m_combatWidget->openAddCharacterDialog();
    } else {
        m_combatWidget->generateTable();
        const auto height = m_combatWidget->getHeight();
        setFixedHeight(std::max(height, START_HEIGHT));
    }

    m_isTableActive = true;
    emit setSaveAction(true);
}


void
MainWindow::setCombatTitle(bool isCombatActive)
{
    if (isWindowModified() == isCombatActive) {
        return;
    }
    QString title = "LCM";
    m_fileName.isEmpty() ? title.append(" - " + (tr("Unnamed Combat"))) : title.append(" - " + m_fileName);
    title.append("[*]");

    setWindowTitle(title);
    setWindowModified(isCombatActive);
}


int
MainWindow::createSaveMessageBox(const QString& tableMessage, bool isClosing)
{
    auto *const msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Question);
    msgBox->setStandardButtons(isWindowModified() ? QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
                                                  : QMessageBox::Yes | QMessageBox::No);
    msgBox->setText(tableMessage);
    msgBox->setWindowTitle(isClosing ? tr("Save and exit?") : tr("Save Combat?"));

    // openTable and newCombat use an identical message box, so do the handling directly
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


QString
MainWindow::createRuleChangeMessageBoxText() const
{
    const auto message = tr("The Table you are trying to load uses another ruleset than you have stored in your rule settings! <br><br>"
                            "Your ruleset: <b>") + Utils::General::getRulesetName(m_ruleSettings.ruleset) + "</b>, " +
                         "<b>" + Utils::General::getAutoRollEnabled(m_ruleSettings.rollAutomatical) + "</b> <br>" +
                         tr("The stored table ruleset is: <b>") + Utils::General::getRulesetName(m_loadedTableRule) + "</b>, " +
                         "<b>" + Utils::General::getAutoRollEnabled(m_loadedTableRollAutomatically) + "</b> <br><br>" +
                         tr("Do you want to apply the stored Table ruleset to your settings or ignore it?");

    return message;
}


void
MainWindow::closeEvent(QCloseEvent *event)
{
    // Check if a table is active and filled
    if (m_isTableActive && isWindowModified()) {
        switch (const auto val = createSaveMessageBox(tr("Currently, you are in a Combat. Do you want "
                                                         "to save the Characters before exiting the program?"), true); val) {
        case QMessageBox::Save:
        {
            saveTable() ? event->accept() : event->ignore();
            break;
        }
        case QMessageBox::Discard:
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        }
    }
}


bool
MainWindow::checkStoredTableRules(const QJsonObject& jsonObjectData)
{
    m_loadedTableRule = static_cast<RuleSettings::Ruleset>(jsonObjectData.value("ruleset").toInt());
    m_loadedTableRollAutomatically = jsonObjectData.value("roll_automatically").toBool();

    return m_ruleSettings.ruleset == m_loadedTableRule &&
           m_ruleSettings.rollAutomatical == m_loadedTableRollAutomatically;
}


void
MainWindow::setMainWindowIcons()
{
    const auto isSystemInDarkMode = Utils::General::isSystemInDarkMode();

    m_newCombatAction->setIcon(QIcon(isSystemInDarkMode ? ":/icons/menus/new_white.svg" : ":/icons/menus/new_black.svg"));
    m_openCombatAction->setIcon(QIcon(isSystemInDarkMode ? ":/icons/menus/open_white.svg" : ":/icons/menus/open_black.svg"));
    m_saveAction->setIcon(QIcon(isSystemInDarkMode ? ":/icons/menus/save_white.svg" : ":/icons/menus/save_black.svg"));
    m_saveAsAction->setIcon(QIcon(isSystemInDarkMode ? ":/icons/menus/save_as_white.svg" : ":/icons/menus/save_as_black.svg"));
    m_openSettingsAction->setIcon(QIcon(isSystemInDarkMode ? ":/icons/menus/gear_white.svg" : ":/icons/menus/gear_black.svg"));
    m_aboutLCMAction->setIcon(QIcon(isSystemInDarkMode ? ":/icons/logos/main_light.svg" : ":/icons/logos/main_dark.svg"));

    QApplication::setWindowIcon(QIcon(isSystemInDarkMode ? ":/icons/logos/main_light.svg" : ":/icons/logos/main_dark.svg"));
}


bool
MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::ApplicationPaletteChange || event->type() == QEvent::PaletteChange) {
        setMainWindowIcons();

        if (m_combatWidget) {
            const auto isSystemInDarkMode = Utils::General::isSystemInDarkMode();
            m_combatWidget->setUndoRedoIcon(isSystemInDarkMode);
        }
    }
    return QWidget::event(event);
}
