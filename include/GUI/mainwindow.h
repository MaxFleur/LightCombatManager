#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "WelcomeWidget.h"
#include "CharacterCreationWidget.h"
#include "TableWidget.h"
#include "../CharacterHandler.h"
#include "../FileHandler.h"

// This class handles the creation and managing of the main GUI window (the dropdown menu and it's actions).
// The layouts themselves are created in other classes and given to this class.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    
private slots:
    // Slots for the dropdown menu actions
    void newCombat();
    void saveTable();
    void openTable();
    void about();
    void aboutQt();
    // Cancel the character creation widget, returning to the welcoming widget
    void cancelCreation();
    // Finish the character creation
    void finishCreation();
    // Exit an existing combat (when the table is displayed)
    void exitCombat();
    // Override the closing function, displaying a message
    virtual void closeEvent ( QCloseEvent * event );

private:
    // Create the user actions and menus
    void createActions();
    void createMenus();
    void setCharacterCreationWidget();
    void setTableWidget(bool isDataStored, QString data = "");
    
    // The widgets displayed when using the program
    WelcomeWidget *welcomeWidget;
    CharacterCreationWidget *characterCreationWidget;
    TableWidget *tableWidget;
    
    // The dropdown menus
    QMenu   *fileMenu;
    QMenu   *helpMenu;
    // The actions when clicking the dropdown menu options
    QAction *newAct;
    QAction *saveAct;
    QAction *openAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    
    // Checks if the user is storing chars 
    bool isCreationActive = false;
    // Checks if the table is displayed
    bool isTableActive = false;

    // The char sort reference used by several widgets
    CharacterHandlerRef m_char;
    // File handler instance to save and open tables
    FileHandlerRef m_file;
};
#endif 
