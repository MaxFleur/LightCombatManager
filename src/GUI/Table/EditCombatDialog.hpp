#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPointer>

class QCheckBox;
class QSpinBox;
class QString;

// Dialog used to add new characters to an existing Combat
class EditCombatDialog : public QDialog {
	Q_OBJECT

public:
	EditCombatDialog(QWidget *parent = 0);

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

	bool m_somethingStored = false;
};
