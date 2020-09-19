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

// Initiate a new combat
void MainWindow::newCombat()
{
    // Allocate the character creation widget, passing the char sort reference to it 
    characterCreationWidget = new CharacterCreationWidget(m_char, this);
    // Then set it to central and connect it's cancel button to the main window
    setCentralWidget(characterCreationWidget);
    connect(characterCreationWidget->getCancelButton(), SIGNAL (clicked ()), this, SLOT (cancelCreation()));
}

// Display about message
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Light Combat Manager"), 
            "Light Combat Manager. A simple Combat Manager for DnD-like games. Code available on Github:"
            "\nhttps://github.com/MaxFleur/LightCombatManager"
            "\n"
            "\nVersion 0.2.9 alpha.");
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
    // If the user clicks yes, remove all created characters
    if (reply == QMessageBox::Yes) {
        m_char->clearCharacters();
        // Then reallocate the welcoming widget and set it to central
        welcomeWidget = new WelcomeWidget(this);
        setCentralWidget(welcomeWidget);
    }
}

MainWindow::~MainWindow()
{
    delete newAct;
    delete aboutAct;
    delete aboutQtAct;
}
