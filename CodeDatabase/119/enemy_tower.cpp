#include "enemy_tower.h"
#include "spell.h"
#include <iostream>

void EnemyTower::update() {
    if(attackCooldown > 0){
        attackCooldown--;
    }
}

void EnemyTower::tryAttack(int targetX, int targetY, Entity* gamer) {
    if (canAttack()) {
        CastContext context = {targetX, targetY, gamer};
        lowSpell.cast(context);
        attackCooldown = maxCooldown;
    }
}

