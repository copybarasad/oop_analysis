#include "CombatMode.h"

const char* CombatModeUtils::toString(CombatMode mode) {
    switch(mode) {
        case CombatMode::MELEE: return "ближний";
        case CombatMode::RANGED: return "дальний";
        default: return "неизвестный";
    }
}

int CombatModeUtils::calculateEffectiveDamage(int baseDamage, CombatMode mode) {
    return (mode == CombatMode::MELEE) ? baseDamage : baseDamage - 5;
}

int CombatModeUtils::getAttackRange(CombatMode mode) {
    switch(mode) {
        case CombatMode::MELEE: return 1;   
        case CombatMode::RANGED: return 3;  
        default: return 1;
    }
}

bool CombatModeUtils::requiresLineOfSight(CombatMode mode) {
    return mode == CombatMode::RANGED;
}