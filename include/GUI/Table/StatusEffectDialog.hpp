#pragma once

#include <QDialog>
#include <QPointer>

class QListWidget;
class QPushButton;

class StatusEffectDialog : public QDialog {
	Q_OBJECT

public:
	StatusEffectDialog(const int	rowNumber,
			   QWidget *	parent = 0);

private:

	void
	findEffect(const QString filter);

	void
	animateFindClick();

	QPointer<QPushButton> m_addEffectButton;
	QPointer<QListWidget> m_list;

	QStringList m_effects = {
		"Bleed",
		"Blinded",
		"Confused",
		"Cowering",
		"Dazed",
		"Dazzled",
		"Deafened",
		"Dying",
		"Energy Drained",
		"Entangled",
		"Disabled",
		"Exhausted",
		"Fascinated",
		"Fatigued",
		"Flat-footed",
		"Frightened",
		"Grappled",
		"Helpless",
		"Incorporeal",
		"Invisible",
		"Nauseated",
		"Panicked",
		"Paralyzed",
		"Petrified",
		"Pinned",
		"Prone",
		"Shaken",
		"Sickened",
		"Stable",
		"Staggered",
		"Stunned",
		"Unconscious"
	};

	int m_rowNumber;
};
