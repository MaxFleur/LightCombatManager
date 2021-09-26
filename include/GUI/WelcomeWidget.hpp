#pragma once

#include <QtWidgets>

// This class handles the welcoming widget
// Which means, it creates a fancy textbox and that's it
class WelcomeWidget : public QWidget {
public:
	WelcomeWidget(QWidget *parent = 0);

private:
	// Create the widget
	void
	createWidget();

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
