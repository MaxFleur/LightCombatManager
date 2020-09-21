#include "../../include/GUI/mainwindow.h"

MainWindow::MainWindow()
{
    // Allocate the char sort object
    m_char = std::make_shared<CharacterHandler>();
    // Set this window as parent for the widgets
    welcomeWidget = new WelcomeWidget(this);
    
    // The welcome widget will be displayed at the beginning, so set to this one
    setCentralWidget(welcomeWidget);
    
    // Create the menus and different actions
    createActions();
    createMenus();
    // Set a title and a minimum size
    setWindowTitle("Light Combat Manager");
    setFixedSize(640, 260);
}

// This function is called if the program is closed
void MainWindow::closeEvent( QCloseEvent *event ) {
    // If a game has been started, send a message if the game should be closed
    if(isGameActive) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              "Exit",
                                                              "You are in a game right now! Do you want to exit the program anyway? All stored characters will be lost.",
                                QMessageBox::Yes|QMessageBox::No);
        // If so, exit the application
        if (reply == QMessageBox::Yes) {
            QApplication::exit;
        // Otherwise ignore this event and nothing happens
        } else {
            event->ignore();
        }
    }
}

// Initiate a new combat
void MainWindow::newCombat()
{
    // Now a game begins, so set to true and reset the widget
    if(!isGameActive) {
        isGameActive = true;
        setCharacterCreationWidget();
    } else {
        // If a game is currently running, asks if a new combat should be started anyway
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              "Start a new combat?",
                                                              "Currently, you are storing characters for a beginning combat. Do you want to create a new combat anyway? "
                                                              "This will delete all characters you have stored for this combat.",
                                QMessageBox::Yes|QMessageBox::No);
        // If so, delete all existing characters and reset the widget
        if (reply == QMessageBox::Yes) {
            m_char->clearCharacters();
            setCharacterCreationWidget();
        }
    }
}

// Display about message
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Light Combat Manager"), 
            "Light Combat Manager. A simple Combat Manager for DnD-like games. Code available on Github:"
            "\nhttps://github.com/MaxFleur/LightCombatManager"
            "\n"
            "\nVersion 0.4.4 alpha.");
}

void MainWindow::aboutQt()
{
}

// Create the user interface actions
void MainWindow::createActions()
{
    // Action to start a new combat
    newAct = new QAction(tr("&New Combat"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Start a new combat."));
    connect(newAct, &QAction::triggered, this, &MainWindow::newCombat);
    // See information about the program
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("About Light Combat Manager"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
    // See information about Qt
    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("About QT version and license"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

// Create the dropdown menus
void MainWindow::createMenus()
{
    // File menu for starting, saving and opening a combat
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addSeparator();
    // Help menu, displays information about the program
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

// This function cancels the character creation widget and returns to the welcoming widget
void MainWindow::cancelCreation() {
    // Create the message box
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              "Cancel creation?",
                                                              "Are you sure you want to cancel the character creation? All created characters will be lost.",
                                QMessageBox::Yes|QMessageBox::No);
    // If the user clicks yes, remove all created characters and set bool to false because no game is active any longer
    if (reply == QMessageBox::Yes) {
        m_char->clearCharacters();
        isGameActive = false;
        // Then reallocate the welcoming widget and set it as the central one
        welcomeWidget = new WelcomeWidget(this);
        setCentralWidget(welcomeWidget);
    }
}

// Finishes the character creation, sorting the created characters and switching to the table layout 
void MainWindow::finishCreation() {
    // If less than 2 characters have been entered, abort
    if(m_char->getCharacters().size() < 2) {
        QMessageBox::StandardButton reply = QMessageBox::warning(this, 
                                                              "Could not finish!",
                                                              "Please store at least 2 characters before creating the combat table!");
        return;
    }
    // Else, display a question message asking if the creation should be finished
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              "Finish creation?",
                                                              "Are you sure you want to finish the character creation? All unsaved inputs will be lost.",
                                QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        // Else start with sorting the characters
        m_char->sortCharacters();
        // Then create the table widget and set it as central
        tableWidget = new TableWidget(m_char, this);
        setCentralWidget(tableWidget);
    }
}

void MainWindow::setCharacterCreationWidget() {
    // Allocate the character creation widget, passing the char sort reference to it 
    characterCreationWidget = new CharacterCreationWidget(m_char, this);
    // Then set it to central and connect it's cancel button to the main window
    setCentralWidget(characterCreationWidget);
    connect(characterCreationWidget->getCancelButton(), SIGNAL (clicked ()), this, SLOT (cancelCreation()));
    connect(characterCreationWidget->getFinishButton(), SIGNAL (clicked ()), this, SLOT (finishCreation()));
}

MainWindow::~MainWindow()
{
    delete newAct;
    delete aboutAct;
    delete aboutQtAct;
}
