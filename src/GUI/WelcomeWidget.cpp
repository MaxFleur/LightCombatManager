#include "../../include/GUI/WelcomeWidget.hpp"

#include <QLabel>
#include <QVBoxLayout>

// Create the welcome widget at instantiation
WelcomeWidget::WelcomeWidget(QWidget *parent)
	: QWidget(parent)
{
	auto *const welcomeLabel =
		new QLabel(
			tr(
				"Welcome to Light Combat Manager! \n To start a new Combat, click on 'File' -> New Combat' or hit Ctrl + N."));
	welcomeLabel->setAlignment(Qt::AlignCenter);

	auto *const mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(welcomeLabel);
}
