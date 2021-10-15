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
		tr("Bleed"),
		tr("Blinded"),
		tr("Confused"),
		tr("Cowering"),
		tr("Dazed"),
		tr("Dazzled"),
		tr("Deafened"),
		tr("Dying"),
		tr("Energy Drained"),
		tr("Entangled"),
		tr("Disabled"),
		tr("Exhausted"),
		tr("Fascinated"),
		tr("Fatigued"),
		tr("Flat-footed"),
		tr("Frightened"),
		tr("Grappled"),
		tr("Helpless"),
		tr("Incorporeal"),
		tr("Invisible"),
		tr("Nauseated"),
		tr("Panicked"),
		tr("Paralyzed"),
		tr("Petrified"),
		tr("Pinned"),
		tr("Prone"),
		tr("Shaken"),
		tr("Sickened"),
		tr("Stable"),
		tr("Staggered"),
		tr("Stunned"),
		tr("Unconscious")
	};

	QString m_effect;
};
