#pragma once

#include <QtWidgets>

// This class handles the welcoming layout
//Which means, it creates a fancy textbox around some widgets and that's it
class WelcomeLayout {
    
public:
    WelcomeLayout();
    // We want this layout to be displayed, so return it, 
    QVBoxLayout *getWelcomeLayout() { return welcomeLayout; }
    
private:
    void createLayout();
    
    // These widgets create some free space around the text in the centre
    QWidget *topFiller;
    QWidget *bottomFiller;
    QWidget *leftFiller;
    QWidget *rightFiller;
    // The label showing the welcome text
    QLabel *welcomeLabel;
    // The overall welcome layout and the layout in the centre
    QVBoxLayout *welcomeLayout;
    QHBoxLayout *centeredLayout;
};
// Shared pointer instance for usage in the mainwindow class
using WelcomeLayoutRef = std::shared_ptr<WelcomeLayout>;
