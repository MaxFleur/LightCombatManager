#include "../../include/GUI/WelcomeWidget.h"

// Create the welcome widget at instantiation
WelcomeWidget::WelcomeWidget(QWidget *parent)
        : QWidget( parent) {
            
    createWidget();
}

// This function creates the welcoming widget
void WelcomeWidget::createWidget() {
    // First, set all filler widgets
    topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    leftFiller = new QWidget;
    leftFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    rightFiller = new QWidget;
    rightFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    // Now create the label with it's text, draw a box around it and aign it to it's centre
    welcomeLabel = new QLabel("Welcome to Light Combat Manager! \n To start a new combat, click on 'File' -> New Combat' or hit Ctrl + N.");
    welcomeLabel->setFrameStyle(QFrame::Box);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    // Now the layouts are allocated, set this widget to the welcomeLayout so it will be shown properly
    welcomeLayout = new QVBoxLayout(this);
    centeredLayout = new QHBoxLayout;
    // Add the left and right filler with the label to the layout in the centre
    centeredLayout->addWidget(leftFiller);
    centeredLayout->addWidget(welcomeLabel);
    centeredLayout->addWidget(rightFiller);
    // Now add the other fillers and this centered layout to the main welcoming layout
    welcomeLayout->setContentsMargins(5, 5, 5, 5);
    welcomeLayout->addWidget(topFiller);
    welcomeLayout->addLayout(centeredLayout);
    welcomeLayout->addWidget(bottomFiller);
}
