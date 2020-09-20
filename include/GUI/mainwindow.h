#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "WelcomeWidget.h"
#include "CharacterCreationWidget.h"
#include "../CharacterHandler.h"

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
    void about();
    void aboutQt();
    // Cancel the character creation widget, returning to the welcoming widget
    void cancelCreation();
    // Finish the character creation
    void finishCreation();

private:
    // Create the user actions and menus
    void createActions();
    void createMenus();
    void setCharacterCreationWidget();
    
    // The widgets displayed when using the program
    WelcomeWidget *welcomeWidget;
    CharacterCreationWidget *characterCreationWidget;
    
    // The dropdown menus
    QMenu   *fileMenu;
    QMenu   *helpMenu;
    // The actions when clicking the dropdown menu options
    QAction *newAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    
    // Checks if the user is storing chars or running a combat
    bool isGameActive = false;
    
    // The char sort reference used by several widgets
    CharacterHandlerRef m_char;
};
#endif 
