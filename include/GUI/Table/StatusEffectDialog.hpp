#pragma once

#include <QDialog>
#include <QPointer>

class QListWidget;
class QPushButton;
class QString;

class StatusEffectDialog : public QDialog {
	Q_OBJECT

public:
	StatusEffectDialog(QWidget *parent = 0);

	QString
	getEffect()
	{
		return m_effect;
	}

private slots:

	void
	addEffect();

private:

	void
	findEffect(const QString filter);

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

	QString m_effect;
};
