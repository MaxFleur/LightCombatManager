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

#include <filesystem>

MainWindow::MainWindow()
{
    // Actions
    m_newCombatAction = new QAction(tr("&New"));
    m_newCombatAction->setShortcuts(QKeySequence::New);
    m_openCombatAction = new QAction(tr("&Open..."));
    m_openCombatAction->setShortcuts(QKeySequence::Open);
    m_saveAction = new QAction(tr("&Save"));
    m_saveAction->setShortcuts(QKeySequence::Save);
    m_saveAsAction = new QAction(tr("&Save As..."));
    m_saveAsAction->setShortcuts(QKeySequence::SaveAs);
    m_openSettingsAction = new QAction(tr("Settings..."));
    m_aboutLCMAction = new QAction(tr("&About LCM"));

    auto* const closeAction = new QAction(QIcon(":/icons/menus/close.svg"), tr("&Close"));
    auto *const aboutQtAction = new QAction(style()->standardIcon(QStyle::SP_TitleBarMenuButton), tr("About &Qt"));

    m_openRecentMenu = new QMenu(tr("Open Recent"));

    // Add actions
    auto *const fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_newCombatAction);
    fileMenu->addAction(m_openCombatAction);
    fileMenu->addMenu(m_openRecentMenu);
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_saveAsAction);
    fileMenu->addAction(closeAction);
    fileMenu->addAction(m_openSettingsAction);
    fileMenu->addSeparator();

    auto *const helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(m_aboutLCMAction);
    helpMenu->addAction(aboutQtAction);

    connect(m_newCombatAction, &QAction::triggered, this, &MainWindow::newCombat);
    connect(m_openCombatAction, &QAction::triggered, this, [this] {
        openTable();
    });
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveTable);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);
    closeAction->setShortcuts(QKeySequence::Close);
    connect(closeAction, &QAction::triggered, this, [this] () {
        m_isTableActive ? exitCombat() : QApplication::quit();
    });
    connect(m_openSettingsAction, &QAction::triggered, this, &MainWindow::openSettings);
    connect(m_aboutLCMAction, &QAction::triggered, this, &MainWindow::about);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
    // Both options have to be enabled or disabled simultaneously
    connect(this, &MainWindow::setSaveAction, this, [this] (bool enable) {
        m_saveAction->setEnabled(enable);
        m_saveAsAction->setEnabled(enable);
    });

    m_tableFileHandler = std::make_shared<TableFileHandler>();

    setOpenRecentMenuActions();
    setMainWindowIcons();
    setWelcomingWidget();
    resize(START_WIDTH, START_HEIGHT);
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
    m_fileDir = QString();

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
        fileName = QFileDialog::getSaveFileName(this, tr("Save Table"),
                                                m_dirSettings.saveDir.isEmpty() ? "combat.lcm" : m_dirSettings.saveDir,
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
    if (m_combatWidget->writeTableToFile(fileName)) {
        m_isTableSavedInFile = true;
        m_dirSettings.write(fileName, true);
        m_fileName = Utils::General::getLCMName(fileName);

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
MainWindow::openTable(const QString& recentDir)
{
    QString fileName;
    if (recentDir.isEmpty()) {
        fileName = QFileDialog::getOpenFileName(this, "Open Table", m_dirSettings.openDir, ("lcm File(*.lcm)"));
        if (fileName.isEmpty()) {
            return;
        }
    } else {
        fileName = recentDir;
    }

    // Return if this exact same file is already loaded
    if ((m_isTableActive && fileName == m_fileDir)) {
        return;
    }

    // Check if a table is active right now
    if (m_isTableActive && isWindowModified() &&
        createSaveMessageBox(tr("Do you want to save the current Combat before opening another existing Combat?"), false) == 0) {
        return;
    }

    auto rulesModified = false;

    switch (const auto code = m_tableFileHandler->getStatus(fileName); code) {
    case 0:
    {
        if (!checkStoredTableRules(m_tableFileHandler->getData())) {
            const auto messageString = createRuleChangeMessageBoxText();
            auto *const msgBox = new QMessageBox(QMessageBox::Warning, tr("Different Rulesets detected!"), messageString, QMessageBox::Cancel);
            auto* const ignoreButton = msgBox->addButton(tr("Use Settings Ruleset"), QMessageBox::AcceptRole);
            auto* const applyButton = msgBox->addButton(tr("Use Table Ruleset (Apply to Settings)"), QMessageBox::ApplyRole);

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
        m_fileName = Utils::General::getLCMName(fileName);
        m_fileDir = fileName;
        setTableWidget(true, false);

        setOpenRecentMenuActions();
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
        QMessageBox::critical(this, tr("Failure reading File!"),
                              tr("The file reading failed. Make sure that the file is accessible and readable."));
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
                       tr("<p>Light Combat Manager. A small, lightweight combat manager for d20-based role playing games.<br>"
                          "<a href='https://github.com/MaxFleur/LightCombatManager'>Code available on Github.</a> Uses GNU GPLv3 license.</p>"
                          "<p>Version 3.1.0.<br>"
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
    setCentralWidget(m_welcomeWidget);
    callTimedResize(START_WIDTH, START_HEIGHT);

    m_isTableSavedInFile = false;
    emit setSaveAction(false);
}


void
MainWindow::setTableWidget(bool isDataStored, bool newCombatStarted)
{
    m_combatWidget = new CombatWidget(m_tableFileHandler, m_additionalSettings, m_ruleSettings, width(), isDataStored, this);
    setCentralWidget(m_combatWidget);
    connect(m_combatWidget, &CombatWidget::exit, this, &MainWindow::exitCombat);
    connect(m_combatWidget, &CombatWidget::tableHeightSet, this, [this] (unsigned int height) {
        callTimedResize(width(), height);
    });
    connect(m_combatWidget, &CombatWidget::tableWidthSet, this, [this] (int tableWidth) {
        callTimedResize(tableWidth, height());
    });
    connect(m_combatWidget, &CombatWidget::changeOccured, this, [this] {
        setCombatTitle(true);
    });

    setCombatTitle(false);

    if (newCombatStarted) {
        callTimedResize(START_WIDTH, START_HEIGHT);
        m_combatWidget->openAddCharacterDialog();
    } else {
        m_combatWidget->generateTableFromTableData();
        const auto width = m_combatWidget->isLoggingWidgetVisible() ? m_combatWidget->width() - 250 : m_combatWidget->width();
        const auto height = m_combatWidget->getHeight();
        callTimedResize(std::max(width, START_WIDTH), std::max(height, START_HEIGHT));
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
    const auto message = tr("The loaded table uses a different ruleset from your settings! <br><br>"
                            "Settings ruleset: <b>") + Utils::General::getRulesetName(m_ruleSettings.ruleset) + "</b>, " +
                         Utils::General::getAutoRollEnabled(m_ruleSettings.rollAutomatical) + "<br>" +
                         tr("Table ruleset: <b>") + Utils::General::getRulesetName(m_loadedTableRule) + "</b>, " +
                         Utils::General::getAutoRollEnabled(m_loadedTableRollAutomatically) + "<br><br>" +
                         tr("Do you want to use your settings or the table ruleset?");

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
MainWindow::setOpenRecentMenuActions()
{
    m_openRecentMenu->clear();

    if (m_dirSettings.recentDirs.at(0).isEmpty()) {
        m_openRecentMenu->addAction(new QAction(tr("No recent dirs")));
    } else {
        for (const auto& recentDir : m_dirSettings.recentDirs) {
            if (!std::filesystem::exists(recentDir.toStdString())) {
                continue;
            }

            auto trimmedName = recentDir;
            if (trimmedName.length() > 50) {
                trimmedName.replace(0, trimmedName.length() - 50, "...");
            }
            auto* const recentDirAction = new QAction(trimmedName);
            m_openRecentMenu->addAction(recentDirAction);
            connect(recentDirAction, &QAction::triggered, this, [this, recentDir] {
                openTable(recentDir);
            });
        }
    }
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

    m_openRecentMenu->setIcon(QIcon(isSystemInDarkMode ? ":/icons/menus/open_white.svg" : ":/icons/menus/open_black.svg"));

    QApplication::setWindowIcon(QIcon(isSystemInDarkMode ? ":/icons/logos/main_light.svg" : ":/icons/logos/main_dark.svg"));
}


void
MainWindow::callTimedResize(int width, int height)
{
    // Sometimes it needs minimal delays to process events in the background before this can be called
    QTimer::singleShot(1, [this, width, height] {
        resize(width, height);
    });
}


bool
MainWindow::event(QEvent *event)
{
    [[unlikely]] if (event->type() == QEvent::ApplicationPaletteChange || event->type() == QEvent::PaletteChange) {
        setMainWindowIcons();

        if (m_combatWidget) {
            const auto isSystemInDarkMode = Utils::General::isSystemInDarkMode();
            m_combatWidget->setUndoRedoIcon(isSystemInDarkMode);
        }
    }
    return QWidget::event(event);
}
