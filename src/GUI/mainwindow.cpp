#include "../../include/GUI/mainwindow.h"

MainWindow::MainWindow()
{
    // Set the main widget and create menus and actions
    welcomeWidget = new WelcomeWidget(this);
    characterCreationWidget = new QWidget;
    setCentralWidget(welcomeWidget);
    
    // Create the layout handlers
    m_charCreateLay = std::make_shared<CharacterCreationLayout>();
    
    // Set the starting layout to the welcome layout, showing the welcome message
    characterCreationWidget->setLayout(m_charCreateLay->getCharacterCreationLayout());
    // Create the menus and different actions
    createActions();
    createMenus();
    // Set a title and a minimum size
    setWindowTitle("Light Combat Manager");
    setFixedSize(640, 260);
}

void MainWindow::newFile()
{
    setCentralWidget(characterCreationWidget);
}

// Display about message
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Light Combat Manager"), 
            "Light Combat Manager. A simple Combat Manager for DnD-like games. "
            "\nCode available on Github: https://github.com/MaxFleur/LightCombatManager"
            "\nVersion 0.1.4 alpha.");
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
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
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

MainWindow::~MainWindow()
{
    delete newAct;
    delete aboutAct;
    delete aboutQtAct;
}
