#pragma once

#include <QObject>

// Contains different status effects based on the used ruleset
namespace StatusEffectData  {
const QList<QStringList> m_specificRulesets = {
    // Pathfinder 1E / D&D 3.5
    {
        QObject::tr("Bleed"),
        QObject::tr("Confused"),
        QObject::tr("Cowering"),
        QObject::tr("Concealed"),
        QObject::tr("Damage Reduction"),
        QObject::tr("Dazed"),
        QObject::tr("Dazzled"),
        QObject::tr("Disabled"),
        QObject::tr("Dying"),
        QObject::tr("Energy Drained"),
        QObject::tr("Energy Resistance"),
        QObject::tr("Entangled"),
        QObject::tr("Exhausted"),
        QObject::tr("Fascinated"),
        QObject::tr("Fatigued"),
        QObject::tr("Flat-footed"),
        QObject::tr("Grappled"),
        QObject::tr("Haste"),
        QObject::tr("Helpless"),
        QObject::tr("Incorporeal"),
        QObject::tr("Invisible"),
        QObject::tr("Nauseated"),
        QObject::tr("Panicked"),
        QObject::tr("Petrified"),
        QObject::tr("Pinned"),
        QObject::tr("Shaken"),
        QObject::tr("Sickened"),
        QObject::tr("Spell Resistance"),
        QObject::tr("Stable"),
        QObject::tr("Staggered")
    },
    // PATHFINDER_2E
    {
        QObject::tr("Clumsy"),
        QObject::tr("Concealed"),
        QObject::tr("Confused"),
        QObject::tr("Controlled"),
        QObject::tr("Dazzled"),
        QObject::tr("Doomed"),
        QObject::tr("Drained"),
        QObject::tr("Dying"),
        QObject::tr("Encumbered"),
        QObject::tr("Enfeebled"),
        QObject::tr("Fascinated"),
        QObject::tr("Fatigued"),
        QObject::tr("Flat-Footed"),
        QObject::tr("Fleeing"),
        QObject::tr("Grabbed"),
        QObject::tr("Hidden"),
        QObject::tr("Immobilized"),
        QObject::tr("Invisible"),
        QObject::tr("Persistent Damage"),
        QObject::tr("Petrified"),
        QObject::tr("Quickened"),
        QObject::tr("Restrained"),
        QObject::tr("Sickened"),
        QObject::tr("Slowed"),
        QObject::tr("Stupefied"),
        QObject::tr("Undetected"),
        QObject::tr("Wounded")
    },
    // D&D 5E
    {
        QObject::tr("Charmed"),
        QObject::tr("Exhaustion - 1"),
        QObject::tr("Exhaustion - 2"),
        QObject::tr("Exhaustion - 3"),
        QObject::tr("Exhaustion - 4"),
        QObject::tr("Exhaustion - 5"),
        QObject::tr("Exhaustion - 6"),
        QObject::tr("Grappled"),
        QObject::tr("Incapacitated"),
        QObject::tr("Invisible"),
        QObject::tr("Petrified"),
        QObject::tr("Poisoned"),
        QObject::tr("Restrained")
    },
    // D&D 3.0
    {
        QObject::tr("Ability Damaged"),
        QObject::tr("Ability Drained"),
        QObject::tr("Blown Away"),
        QObject::tr("Checked"),
        QObject::tr("Confused"),
        QObject::tr("Cowering"),
        QObject::tr("Dazed"),
        QObject::tr("Dazzled"),
        QObject::tr("Dead"),
        QObject::tr("Disabled"),
        QObject::tr("Dying"),
        QObject::tr("Energy Drained"),
        QObject::tr("Entangled"),
        QObject::tr("Exhausted"),
        QObject::tr("Fatigued"),
        QObject::tr("Flat-Footed"),
        QObject::tr("Grappling"),
        QObject::tr("Held"),
        QObject::tr("Helpless"),
        QObject::tr("Incapacitated"),
        QObject::tr("Incorporeal"),
        QObject::tr("Invisible"),
        QObject::tr("Knocked Down"),
        QObject::tr("Nauseated"),
        QObject::tr("Normal"),
        QObject::tr("Panicked"),
        QObject::tr("Petrified"),
        QObject::tr("Pinned"),
        QObject::tr("Shaken"),
        QObject::tr("Stable"),
        QObject::tr("Staggered"),
        QObject::tr("Turned")
    },
    // Starfinder
    {
        QObject::tr("Asleep"),
        QObject::tr("Blinded"),
        QObject::tr("Broken"),
        QObject::tr("Burning"),
        QObject::tr("Confused"),
        QObject::tr("Cowering"),
        QObject::tr("Dazed"),
        QObject::tr("Dazzled"),
        QObject::tr("Dead"),
        QObject::tr("Dying"),
        QObject::tr("Encumbered"),
        QObject::tr("Entangled"),
        QObject::tr("Exhausted"),
        QObject::tr("Fascinated"),
        QObject::tr("Fatigued"),
        QObject::tr("Flat-Footed"),
        QObject::tr("Grappled"),
        QObject::tr("Helpless"),
        QObject::tr("Nauseated"),
        QObject::tr("Off-Kilter"),
        QObject::tr("Off-Target"),
        QObject::tr("Overburdened"),
        QObject::tr("Panicked"),
        QObject::tr("Pinned"),
        QObject::tr("Shaken"),
        QObject::tr("Sickened"),
        QObject::tr("Stable"),
        QObject::tr("Staggered")
    }
};

// Common effects used in all rulesets
const QStringList m_commonEffects = {
    QObject::tr("Blinded"),
    QObject::tr("Deafened"),
    QObject::tr("Frightened"),
    QObject::tr("Paralyzed"),
    QObject::tr("Prone"),
    QObject::tr("Stunned"),
    QObject::tr("Unconscious")
};

[[nodiscard]] QStringList
getEffectList(unsigned int index)
{
    auto list = m_commonEffects;
    list << m_specificRulesets[index];
    list.sort();
    return list;
}
}
