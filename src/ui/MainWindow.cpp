#include "MainWindow.hpp"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QKeySequence>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTimer>

#include "CombatWidget.hpp"
#include "SettingsDialog.hpp"
#include "UtilsGeneral.hpp"
#include "UtilsTable.hpp"
#include "WelcomeWidget.hpp"

MainWindow::MainWindow()
{
    // Actions
    auto *const newCombatAction = new QAction(style()->standardIcon(QStyle::SP_FileIcon), tr("&New"), this);
    newCombatAction->setShortcuts(QKeySequence::New);
    connect(newCombatAction, &QAction::triggered, this, &MainWindow::newCombat);

    auto *const openTableAction = new QAction(style()->standardIcon(QStyle::SP_DirOpenIcon), tr("&Open..."), this);
    openTableAction->setShortcuts(QKeySequence::Open);
    connect(openTableAction, &QAction::triggered, this, &MainWindow::openTable);

    auto *const closeTableAction = new QAction(style()->standardIcon(QStyle::SP_TitleBarCloseButton), tr("&Close"), this);
    closeTableAction->setShortcuts(QKeySequence::Close);
    connect(closeTableAction, &QAction::triggered, this, [this] () {
        m_isTableActive ? exitCombat() : QApplication::quit();
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

    const auto isSystemInDarkMode = Utils::General::isColorDark(this->palette().color(QPalette::Window));
    m_openSettingsAction = new QAction(isSystemInDarkMode ? QIcon(":/icons/gear_white.png") : QIcon(":/icons/gear_black.png"),
                                       tr("Settings..."), this);
    connect(m_openSettingsAction, &QAction::triggered, this, &MainWindow::openSettings);

    auto *const aboutAction = new QAction(style()->standardIcon(QStyle::SP_DialogHelpButton), tr("&About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

    auto *const aboutQtAction = new QAction(style()->standardIcon(QStyle::SP_TitleBarMenuButton), tr("About &Qt"), this);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

    // Add actions
    auto *const fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newCombatAction);
    fileMenu->addAction(openTableAction);
    fileMenu->addAction(closeTableAction);
    fileMenu->addAction(saveTableAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(m_openSettingsAction);
    fileMenu->addSeparator();

    auto *const helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    m_file = std::make_shared<FileHandler>();

    resize(START_WIDTH, START_HEIGHT);
    setWelcomingWidget();
}


void
MainWindow::newCombat()
{
    // Check if a table is active
    if (m_isTableActive && !m_combatWidget->isEmpty()) {
        if (createSaveMessageBox(isWindowModified() ?
                                 tr("Do you want to save the current Combat before starting a new one?") :
                                 tr("Do you want to start a new Combat?"), false) == 0) {
            return;
        }
    }
    m_tableInFile = false;

    m_fileName = QString();
    setCombatTitle(false);

    setTableWidget(false, true);
}


bool
MainWindow::saveTable()
{
    if (!isWindowModified()) {
        return false;
    }

    if (m_combatWidget->isEmpty()) {
        auto const reply = QMessageBox::critical(this, tr("Table empty!"),
                                                 tr("Can't save an empty table."));
        return false;
    }
    if (Utils::General::containsSemicolon(m_combatWidget->getTableWidget())) {
        auto const reply = QMessageBox::critical(this, tr("Semicolons detected!"),
                                                 tr("Can't save because the table contains semicolons. Please remove them and continue."));
        return false;
    }

    QString fileName;
    // Save to standard save dir if a new combat has been started
    if (!m_tableInFile) {
        fileName = QFileDialog::getSaveFileName(this, tr("Save Table"), m_dirSettings.saveDir,
                                                tr("Table (*.csv);;All Files (*)"));

        if (fileName.isEmpty()) {
            // No file provided or Cancel pressed
            return false;
        }
        // Otherwise, just overwrite the loaded file
    } else {
        fileName = m_dirSettings.openDir;
    }
    // Save the table
    const auto tableDataWidget = Utils::Table::tableDataFromWidget(m_combatWidget->getTableWidget());
    if (m_file->saveTable(tableDataWidget, fileName, m_combatWidget->getRowEntered(),
                          m_combatWidget->getRoundCounter(), m_ruleSettings.ruleset, m_ruleSettings.rollAutomatical)) {
        m_tableInFile = true;
        m_dirSettings.write(fileName, true);
        m_fileName = Utils::General::getCSVName(fileName);

        setCombatTitle(false);
        // Success
        return true;
    }
    auto const reply = QMessageBox::critical(this, tr("Could not save table!"), tr("Failed to write file."));
    return false;
}


// Reopen the file dialog, even if the table has been saved already
void
MainWindow::saveAs()
{
    // Save state
    const auto saveChangeOccured = isWindowModified();
    const auto saveTableInFile = m_tableInFile;

    // Change variables to call the file dialog
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
    if (m_isTableActive && !m_combatWidget->isEmpty()) {
        if (createSaveMessageBox(isWindowModified() ?
                                 tr("Do you want to save the current Combat before opening another existing Combat?") :
                                 tr("Do you want to open another existing Combat?"), false) == 0) {
            return;
        }
    }
    const auto fileName = QFileDialog::getOpenFileName(this, "Open Table", m_dirSettings.openDir, ("csv File(*.csv)"));
    const auto code = m_file->getCSVData(fileName);
    auto rulesModified = false;

    switch (code) {
    case 0:
    {
        if (!checkStoredTableRules(m_file->getData())) {
            const auto messageString = createRuleChangeMessageBoxText();
            auto *const msgBox = new QMessageBox(QMessageBox::Warning, tr("Different rulesets detected!"), messageString, QMessageBox::Cancel);
            auto* const applyButton = msgBox->addButton(tr("Apply Table ruleset to Settings"), QMessageBox::ApplyRole);
            auto* const ignoreButton = msgBox->addButton(tr("Ignore Stored Table ruleset"), QMessageBox::AcceptRole);

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
        m_tableInFile = true;
        // Save the opened file dir
        m_dirSettings.write(fileName);
        m_fileName = Utils::General::getCSVName(fileName);
        setTableWidget(true, false, m_file->getData());

        // If the settings rules are applied to the table, it is modified
        setCombatTitle(rulesModified);
        break;
    }
    case 1:
    {
        auto const reply = QMessageBox::critical(this, tr("Wrong Table format!"),
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
    dialog->show();
}


void
MainWindow::about()
{
    QMessageBox::about(this, tr("About Light Combat Manager"),
                       tr("<p>Light Combat Manager. A small, lightweight Combat Manager for d20-based role playing games.<br>"
                          "<a href='https://github.com/MaxFleur/LightCombatManager'>Code available on Github.</a></p>"
                          "<p>Version 1.11.0.<br>"
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
    m_combatWidget = new CombatWidget(m_additionalSettings, m_ruleSettings, this->width(), isDataStored, data, this);
    setCentralWidget(m_combatWidget);
    connect(m_combatWidget, &CombatWidget::exit, this, &MainWindow::exitCombat);
    connect(m_combatWidget, &CombatWidget::tableHeightSet, this, [this] (int height) {
        if (height > START_HEIGHT) {
            setFixedHeight(height);
        }
    });
    connect(m_combatWidget, &CombatWidget::tableWidthSet, this, [this] (int width) {
        if (width > this->width()) {
            // @note A single immediate call to resize() won't actually resize the window
            // So the function is called with a minimal delay of 1 ms, which will actually
            // resize the main window
            QTimer::singleShot(1, [this, width]() {
                resize(width, this->height());
            });
        }
    });
    connect(m_combatWidget, &CombatWidget::changeOccured, this, [this] () {
        setCombatTitle(true);
    });

    setCombatTitle(false);

    if (newCombatStarted) {
        setFixedHeight(START_HEIGHT);
        m_combatWidget->openAddCharacterDialog();
    } else {
        m_combatWidget->generateTable();
        const auto height = m_combatWidget->getHeight();
        setFixedHeight(height > START_HEIGHT ? height : START_HEIGHT);
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
    msgBox->setStandardButtons(isWindowModified() ?
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel :
                               QMessageBox::Yes | QMessageBox::No);
    msgBox->setText(tableMessage);
    if (isClosing) {
        isWindowModified() ? msgBox->setWindowTitle(tr("Save and exit?")) : msgBox->setWindowTitle(tr("Exit application?"));
    } else {
        msgBox->setWindowTitle(tr("Save Combat?"));
    }

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
    if (m_isTableActive && !m_combatWidget->isEmpty()) {
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
        case QMessageBox::Yes:
            event->accept();
            break;
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

    return m_ruleSettings.ruleset == m_loadedTableRule &&
           m_ruleSettings.rollAutomatical == m_loadedTableRollAutomatically;
}


bool
MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::ApplicationPaletteChange || event->type() == QEvent::PaletteChange) {
        const auto isSystemInDarkMode = Utils::General::isColorDark(this->palette().color(QPalette::Window));
        m_openSettingsAction->setIcon(isSystemInDarkMode ? QIcon(":/icons/gear_white.png") : QIcon(":/icons/gear_black.png"));

        if (m_combatWidget) {
            m_combatWidget->setUndoRedoIcon(isSystemInDarkMode);
        }
    }
    return QWidget::event(event);
}
