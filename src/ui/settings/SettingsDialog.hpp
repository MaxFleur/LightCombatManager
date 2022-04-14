#pragma once

#include <QDialog>

// Dialog used to add certain status effects to characters
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
