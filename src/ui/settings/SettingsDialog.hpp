#pragma once

#include <QDialog>
#include <QPointer>

class SettingsData;

class QCheckBox;
class QComboBox;

// Dialog for the program settings
class SettingsDialog : public QDialog {
	Q_OBJECT

public:
	explicit
	SettingsDialog(std::shared_ptr<SettingsData>	settingsData,
		       bool				isTableActive,
		       QWidget *			parent = 0);

private slots:

	bool
	applyClicked();

	void
	okClicked();

private:

	QPointer<QComboBox> m_rulesetBox;
	QPointer<QCheckBox> m_rollTieBox;

	std::shared_ptr<SettingsData> m_settingsData;
	bool m_isTableActive;
};
