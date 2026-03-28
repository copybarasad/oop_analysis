#ifndef COMBATMODE_H
#define COMBATMODE_H

enum class CombatMode {
    MELEE,
    RANGED
};

namespace CombatModeUtils {
    const char* toString(CombatMode mode);
    int calculateEffectiveDamage(int baseDamage, CombatMode mode);
    int getAttackRange(CombatMode mode);
    bool requiresLineOfSight(CombatMode mode);
}

#endif