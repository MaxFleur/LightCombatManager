#include "../../include/GUI/WelcomeWidget.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QWidget>

// Create the welcome widget at instantiation
WelcomeWidget::WelcomeWidget(QWidget *parent)
	: QWidget(parent)
{
	// Side widgets
	auto *topWidget = new QWidget;
	topWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	auto *bottomWidget = new QWidget;
	bottomWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	auto *leftWidget = new QWidget;
	leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto *rightWidget = new QWidget;
	rightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	// Create text label
	auto *welcomeLabel =
		new QLabel(
			tr(
				"Welcome to Light Combat Manager! \n To start a new combat, click on 'File' -> New Combat' or hit Ctrl + N."));
	welcomeLabel->setFrameStyle(QFrame::Box);
	welcomeLabel->setAlignment(Qt::AlignCenter);

	// Create centered layout
	auto *centeredLayout = new QHBoxLayout;
	centeredLayout->addWidget(leftWidget);
	centeredLayout->addWidget(welcomeLabel);
	centeredLayout->addWidget(rightWidget);

	// Create main layout and add widgets
	auto *mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(5, 5, 5, 5);
	mainLayout->addWidget(topWidget);
	mainLayout->addLayout(centeredLayout);
	mainLayout->addWidget(bottomWidget);
}
