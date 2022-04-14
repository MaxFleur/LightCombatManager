#pragma once

#include <QDialog>

// Dialog for the program settings
class SettingsDialog : public QDialog {
	Q_OBJECT

public:
	explicit
	SettingsDialog(QWidget *parent = 0);

private slots:

	void
	applyClicked();

	void
	okClicked();
};
