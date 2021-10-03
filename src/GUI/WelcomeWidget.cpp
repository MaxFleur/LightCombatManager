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
	auto *const topWidget = new QWidget;
	topWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	auto *const bottomWidget = new QWidget;
	bottomWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	auto *const leftWidget = new QWidget;
	leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto *const rightWidget = new QWidget;
	rightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto *const welcomeLabel =
		new QLabel(
			tr(
				"Welcome to Light Combat Manager! \n To start a new combat, click on 'File' -> New Combat' or hit Ctrl + N."));
	welcomeLabel->setFrameStyle(QFrame::Box);
	welcomeLabel->setAlignment(Qt::AlignCenter);

	auto *const centeredLayout = new QHBoxLayout;
	centeredLayout->addWidget(leftWidget);
	centeredLayout->addWidget(welcomeLabel);
	centeredLayout->addWidget(rightWidget);

	auto *const mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(5, 5, 5, 5);
	mainLayout->addWidget(topWidget);
	mainLayout->addLayout(centeredLayout);
	mainLayout->addWidget(bottomWidget);
}
