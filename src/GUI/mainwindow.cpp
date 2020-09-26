#include "../../include/GUI/mainwindow.h"

MainWindow::MainWindow()
{
    // Allocate the char sort object
    m_char = std::make_shared<CharacterHandler>();
    // Same for file handler
    m_file = std::make_shared<FileHandler>();
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
    if(isCreationActive) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              "Exit",
                                                              "You are in the character creation right now! Do you want to exit the program anyway? All stored characters will be lost.",
                                QMessageBox::Yes|QMessageBox::No);
        // If so, exit the application
        if (reply == QMessageBox::Yes) {
            QApplication::exit;
        // Otherwise ignore this event and nothing happens
        } else {
            event->ignore();
        }
    }
    // If the table is active, send a question if the table should be saved
    if(isTableActive) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              "Exit",
                                                              "Currently, you are in a combat. Do you want to save the characters before exiting the program?",
                                QMessageBox::Yes|QMessageBox::No);
        // If so, save the table
        if (reply == QMessageBox::Yes) {
            saveTable();
        } 
        // Exit
        QApplication::exit;
    }
}

// Initiate a new combat
void MainWindow::newCombat()
{
    // Now a game begins, so set to true and reset the widget
    if(isCreationActive) {
        // If a game is currently running, asks if a new combat should be started anyway
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              "Start a new combat?",
                                                              "Currently, you are storing characters for a beginning combat. Do you want to create a new combat anyway? "
                                                              "This will delete all characters you have stored for this combat.",
                                QMessageBox::Yes|QMessageBox::No);
        // If so, delete all existing characters and reset the widget
        if (reply == QMessageBox::Yes) {
            m_char->clearCharacters();
        } else {
            return;
        }
    // If the table is active right now, asks if the current table should be saved
    } else if (isTableActive) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              "Start a new combat?",
                                                              "Do you want to save the current combat before starting a new combat?",
                                QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        // If so, save and set the table active to false
        if (reply == QMessageBox::Yes) {
            saveTable();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
        isTableActive = false;
    }
    // Then set to the character creation widget
    isCreationActive = true;
    setCharacterCreationWidget();
}

// Save the table
void MainWindow::saveTable() {
    // Set a filename
    QString filename = QFileDialog::getSaveFileName(this,
        "Save Table", QDir::currentPath(),
        "Table (*.csv);;All Files (*)");
    // Return if no filename is provided
    if (filename.isEmpty()) {
        return;
    } 
    // Otherwise store the table
    m_file->saveTable(tableWidget->getTableWidget(), filename);
}

// Open the table
void MainWindow::openTable() {
    if(isCreationActive) {
        // If a game is currently running, asks if a table shoud be opened anyway
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              "Open table??",
                                                              "Currently, you are storing characters for a beginning combat. Do you want to open another table anyway?",
                                QMessageBox::Yes|QMessageBox::No);
        // If so, delete all existing characters
        if (reply == QMessageBox::Yes) {
            m_char->clearCharacters();
        } else {
            return;
        }
    // If the table is active right now, asks if the current table should be saved
    } else if (isTableActive) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              "Save current table?",
                                                              "Do you want to save the current combat before opening another table?",
                                QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        // If so, save and set the table active to false
        if (reply == QMessageBox::Yes) {
            saveTable();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }
    // Get the file path
    QString filename = QFileDialog::getOpenFileName(this, "Open Table", QDir::currentPath(), ("csv File(*.csv)"));
    // Test if data can be read using the filename
    if(m_file->getCSVData(filename)) {
        // Table and char creation not active for a short time
        isCreationActive = false;
        isTableActive = false;
        // If the data collection was successful, create the table widget and set it as central
        setTableWidget(true, m_file->getData());
    }
}

// Display about message
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Light Combat Manager"), 
            "Light Combat Manager. A simple Combat Manager for DnD-like games. Code available on Github:"
            "\nhttps://github.com/MaxFleur/LightCombatManager"
            "\n"
            "\nVersion 0.5.11 alpha.");
}

void MainWindow::aboutQt()
{
}

// Create the user interface actions
void MainWindow::createActions()
{
    // Action to start a new combat
    newAct = new QAction("&New Combat", this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip("Start a new combat.");
    connect(newAct, &QAction::triggered, this, &MainWindow::newCombat);
    
    // Action to save the table
    saveAct = new QAction("&Save Table", this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip("Save the created table.");
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveTable);
    
    // Action to open the table
    openAct = new QAction("&Open Table", this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip("Open an existing table.");
    connect(openAct, &QAction::triggered, this, &MainWindow::openTable);
    
    // See information about the program
    aboutAct = new QAction("&About", this);
    aboutAct->setStatusTip("About Light Combat Manager");
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
    
    // See information about Qt
    aboutQtAct = new QAction("About &Qt", this);
    aboutQtAct->setStatusTip("About QT version and license");
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
    
    // Because no table has been created yet, disable this action
    saveAct->setEnabled(false);
}

// Create the dropdown menus
void MainWindow::createMenus()
{
    // File menu for starting, saving and opening a combat
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(newAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    // Help menu, displays information about the program
    helpMenu = menuBar()->addMenu("&Help");
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
        isCreationActive = false;
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
        // Now the creation is no longer active, so set to false
        isCreationActive = false;
        // Else start with sorting the characters
        m_char->sortCharacters();
        // Then create the table widget and set it as central
        setTableWidget(false);
    }
    // Now that a table has been created, enable the save action
    saveAct->setEnabled(true);
}

// Exits the combat and returns to the welcoming widget
void MainWindow::exitCombat() {
    // Standard reply, asking if the user is sure
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              "Exit combat?",
                                                              "Are you sure you want to exit the combat? All created characters will be lost.",
                                QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        // If so, delete all created characters and set active game to false
        m_char->clearCharacters();
        // Then reallocate and set the welcoming widget
        welcomeWidget = new WelcomeWidget(this);
        setCentralWidget(welcomeWidget);
        // The table has been destroyed, so disable the saving action
        saveAct->setEnabled(false);
        isTableActive = false;
    }
}

// Helper function for setting the character creation widget
void MainWindow::setCharacterCreationWidget() {
    // Allocate the character creation widget, passing the char sort reference to it 
    characterCreationWidget = new CharacterCreationWidget(m_char, this);
    // Then set it to central and connect it's cancel button to the main window
    setCentralWidget(characterCreationWidget);
    connect(characterCreationWidget->getCancelButton(), SIGNAL (clicked ()), this, SLOT (cancelCreation()));
    connect(characterCreationWidget->getFinishButton(), SIGNAL (clicked ()), this, SLOT (finishCreation()));
}

// Helper function for setting the table widget
void MainWindow::setTableWidget(bool isDataStored, QString data) {
    tableWidget = new TableWidget(m_char, this);
    // Set the data depending on if the data has been stored as a csv or not
    if(isDataStored) {
        tableWidget->setTableData(true, data);
    } else {
        tableWidget->setTableData(false);
    }
    setCentralWidget(tableWidget);
    connect(tableWidget->getExitButton(), SIGNAL (clicked ()), this, SLOT (exitCombat()));
    // Now the table is active
    isTableActive = true;
}

MainWindow::~MainWindow()
{
    delete newAct;
    delete saveAct;
    delete openAct;
    delete aboutAct;
    delete aboutQtAct;
}
