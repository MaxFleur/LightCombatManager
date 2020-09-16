#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "WelcomeLayout.h"
#include "CharacterCreationLayout.h"

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
    void newFile();
    void about();
    void aboutQt();

private:
    // Create the user actions and menus
    void createActions();
    void createMenus();
    
    WelcomeLayoutRef m_welcLay;
    CharacterCreationLayoutRef m_charCreateLay;
    
    // The main widget used for the GUI
    QWidget *welcomeWidget;
    QWidget *characterCreationWidget;
    
    // The dropdown menus
    QMenu   *fileMenu;
    QMenu   *helpMenu;
    // The actions when clicking the dropdown menu options
    QAction *newAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
};
#endif 
