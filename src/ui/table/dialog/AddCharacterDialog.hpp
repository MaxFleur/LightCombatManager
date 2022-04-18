#pragma once

#include <QDialog>
#include <QPointer>

class QCheckBox;
class QLabel;
class QLineEdit;
class QMessageBox;
class QSpinBox;

class SettingsData;

// Dialog used to add new characters to an existing Combat
class AddCharacterDialog : public QDialog {
	Q_OBJECT

public:
	explicit
	AddCharacterDialog(std::shared_ptr<SettingsData>	settingsData,
			   QWidget *				parent = 0);

signals:

	void
	characterCreated(QString	name,
			 int		ini,
			 int		mod,
			 int		hp,
			 bool		isEnemy,
			 QString	addInfo);

private slots:

	void
	setLabelRolled();

	void
	animateLabel();

	void
	addCharacter();

	void
	resetValues();

	void
	openStatusEffectDialog();

private:

	void
	setFocus();

	void
	keyPressEvent(QKeyEvent *event);

	void
	closeEvent(QCloseEvent *event);

	QPointer<QLineEdit> m_nameEdit;
	QPointer<QSpinBox> m_iniBox;
	QPointer<QSpinBox> m_iniModifierBox;
	QPointer<QLabel> m_labelRolled;
	QPointer<QSpinBox> m_hpBox;
	QPointer<QCheckBox> m_enemyBox;
	QPointer<QLineEdit> m_addInfoEdit;

	QPointer<QLabel> m_animatedLabel;
	QPointer<QTimer> m_timer;

	bool m_somethingStored{ false };
	bool m_isFirstCharStored{ false };

	std::shared_ptr<SettingsData> m_settingsData;

	static constexpr int LABEL_SHOWN = 1000;
	static constexpr int LABEL_FADEOUT = 2000;
};
