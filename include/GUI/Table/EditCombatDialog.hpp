#pragma once

#include <QDialog>
#include <QPointer>

class QCheckBox;
class QLineEdit;
class QSpinBox;

class EditCombatDialog : public QDialog {
	Q_OBJECT

public:
	EditCombatDialog(QWidget *parent = 0);

private:

	QPointer<QLineEdit> m_nameEdit;
	QPointer<QSpinBox> m_iniBox;
	QPointer<QSpinBox> m_iniModifierBox;
	QPointer<QSpinBox> m_hpBox;
	QPointer<QCheckBox> m_npcBox;
	QPointer<QLineEdit> m_addInfoEdit;
};
