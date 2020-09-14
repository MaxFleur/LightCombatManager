#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

// This class handles the creation and managing of the main GUI window.
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
    
    // The main widget used for the GUI
    QWidget *widget;
    // The dropdown menus
    QMenu *fileMenu;
    QMenu *helpMenu;
    // The actions when clicking the dropdown menu options
    QAction *newAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
};
#endif 
