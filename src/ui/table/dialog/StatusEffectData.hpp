#pragma once

#include <QObject>

// Contains different status effects based on the used ruleset
namespace StatusEffectData  {
const QList<QStringList> m_effects = {
	// Pathfinder 1E / D&D 3.5
	{
		QObject::tr("Bleed"),
		QObject::tr("Blinded"),
		QObject::tr("Confused"),
		QObject::tr("Cowering"),
		QObject::tr("Concealed"),
		QObject::tr("Damage Reduction"),
		QObject::tr("Dazed"),
		QObject::tr("Dazzled"),
		QObject::tr("Deafened"),
		QObject::tr("Disabled"),
		QObject::tr("Dying"),
		QObject::tr("Energy Drained"),
		QObject::tr("Energy Resistance"),
		QObject::tr("Entangled"),
		QObject::tr("Exhausted"),
		QObject::tr("Fascinated"),
		QObject::tr("Fatigued"),
		QObject::tr("Flat-footed"),
		QObject::tr("Frightened"),
		QObject::tr("Grappled"),
		QObject::tr("Haste"),
		QObject::tr("Helpless"),
		QObject::tr("Incorporeal"),
		QObject::tr("Invisible"),
		QObject::tr("Nauseated"),
		QObject::tr("Panicked"),
		QObject::tr("Paralyzed"),
		QObject::tr("Petrified"),
		QObject::tr("Pinned"),
		QObject::tr("Prone"),
		QObject::tr("Shaken"),
		QObject::tr("Sickened"),
		QObject::tr("Spell Resistance"),
		QObject::tr("Stable"),
		QObject::tr("Staggered"),
		QObject::tr("Stunned"),
		QObject::tr("Unconscious")
	},
	// PATHFINDER_2E
	{
		QObject::tr("Blinded"),
		QObject::tr("Clumsy"),
		QObject::tr("Concealed"),
		QObject::tr("Confused"),
		QObject::tr("Controlled"),
		QObject::tr("Dazzled"),
		QObject::tr("Deafened"),
		QObject::tr("Doomed"),
		QObject::tr("Drained"),
		QObject::tr("Dying"),
		QObject::tr("Encumbered"),
		QObject::tr("Enfeebled"),
		QObject::tr("Fascinated"),
		QObject::tr("Fatigued"),
		QObject::tr("Flat-Footed"),
		QObject::tr("Fleeing"),
		QObject::tr("Frightened"),
		QObject::tr("Grabbed"),
		QObject::tr("Hidden"),
		QObject::tr("Immobilized"),
		QObject::tr("Invisible"),
		QObject::tr("Paralyzed"),
		QObject::tr("Persistent Damage"),
		QObject::tr("Petrified"),
		QObject::tr("Prone"),
		QObject::tr("Quickened"),
		QObject::tr("Restrained"),
		QObject::tr("Sickened"),
		QObject::tr("Slowed"),
		QObject::tr("Stunned"),
		QObject::tr("Stupefied"),
		QObject::tr("Unconscious"),
		QObject::tr("Undetected"),
		QObject::tr("Wounded")
	},
	// D&D 5E
	{
		QObject::tr("Blinded"),
		QObject::tr("Charmed"),
		QObject::tr("Deafened"),
		QObject::tr("Exhaustion - 1"),
		QObject::tr("Exhaustion - 2"),
		QObject::tr("Exhaustion - 3"),
		QObject::tr("Exhaustion - 4"),
		QObject::tr("Exhaustion - 5"),
		QObject::tr("Exhaustion - 6"),
		QObject::tr("Frightened"),
		QObject::tr("Grappled"),
		QObject::tr("Incapacitated"),
		QObject::tr("Invisible"),
		QObject::tr("Paralyzed"),
		QObject::tr("Petrified"),
		QObject::tr("Poisoned"),
		QObject::tr("Prone"),
		QObject::tr("Restrained"),
		QObject::tr("Stunned"),
		QObject::tr("Unconscious")
	}
};

[[nodiscard]] QStringList
getEffectList(int index)
{
	return m_effects[index];
}
}
