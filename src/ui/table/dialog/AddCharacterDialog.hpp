#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPointer>

class QCheckBox;
class QSpinBox;

// Dialog used to add new characters to an existing Combat
class AddCharacterDialog : public QDialog {
	Q_OBJECT

public:
	AddCharacterDialog(QWidget *parent = 0);

signals:

	void
	characterCreated(QString	name,
			 int		ini,
			 int		mod,
			 int		hp,
			 bool		isNPC,
			 QString	addInfo);

private slots:

	void
	addCharacter();

	void
	resetLayout();

	void
	openStatusEffectDialog();

	void
	animateLabel();

private:

	void
	setFocus()
	{
		m_nameEdit->setFocus(Qt::TabFocusReason);
	}

	void
	keyPressEvent(QKeyEvent *event);

	void
	closeEvent(QCloseEvent *event);

	QPointer<QLineEdit> m_nameEdit;
	QPointer<QSpinBox> m_iniBox;
	QPointer<QSpinBox> m_iniModifierBox;
	QPointer<QSpinBox> m_hpBox;
	QPointer<QCheckBox> m_npcBox;
	QPointer<QLineEdit> m_addInfoEdit;

	QPointer<QLabel> m_animatedLabel;
	QPointer<QTimer> m_timer;

	bool m_somethingStored{ false };
	bool m_isFirstCharStored{ false };

	static constexpr int LABEL_SHOWN = 1000;
	static constexpr int LABEL_FADEOUT = 2000;
};
