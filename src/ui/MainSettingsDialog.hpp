#pragma once

#include <QDialog>
#include <QPointer>

class MainSettings;

class QCheckBox;
class QComboBox;

// Dialog for the program settings
class MainSettingsDialog : public QDialog {
	Q_OBJECT

public:
	explicit
	MainSettingsDialog(std::shared_ptr<MainSettings>	MainSettings,
			   bool					isTableActive,
			   QWidget *				parent = 0);

private slots:

	bool
	applyClicked();

	void
	okClicked();

private:

	QPointer<QComboBox> m_rulesetBox;
	QPointer<QCheckBox> m_rollTieBox;

	std::shared_ptr<MainSettings> m_mainSettings;
	bool m_isTableActive;
};
