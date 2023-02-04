#include "WelcomeWidget.hpp"

#include <QLabel>
#include <QVBoxLayout>

WelcomeWidget::WelcomeWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *const welcomeLabel = new QLabel(tr("Welcome to Light Combat Manager! \n To start a new Combat, click on 'File' -> "
                                             "'New Combat' or hit Ctrl + N."));
    welcomeLabel->setAlignment(Qt::AlignCenter);

    auto *const layout = new QVBoxLayout(this);
    layout->addWidget(welcomeLabel);
    setLayout(layout);
}
