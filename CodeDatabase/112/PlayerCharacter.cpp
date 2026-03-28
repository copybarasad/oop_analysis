#include "PlayerCharacter.h"
#include "GameGrid.h"
#include "SpellCard.h"
#include <iostream>

void PlayerCharacter::setBattleMode(BattleMode mode) {
    currentMode = mode;
    if (currentMode == BattleMode::CLOSE_COMBAT) {
        setAttackRange(closeCombatRange);
    } else {
        setAttackRange(distanceCombatRange);
    }
}

void PlayerCharacter::setCloseDamage(int damage) {
    closeCombatDamage = damage;
}

void PlayerCharacter::setDistanceDamage(int damage) {
    distanceCombatDamage = damage;
}
