#include "EnemyTower.h"
#include "Player.h"
#include "GameField.h"
#include "CombatManager.h"
#include "Constants.h"
#include <iostream>

EnemyTower::EnemyTower(int x, int y, int range, int damage, int cooldown)
    : EnemyBuilding(x, y, DEFAULT_TOWER_HEALTH, damage), 
      attackRange(range),
      attackCooldown(cooldown),
      currentCooldown(0) {}

void EnemyTower::update() {
    if (currentCooldown > 0) {
        currentCooldown--;
    }
}

void EnemyTower::tryAttack(Player& player, GameField& /*field*/) {
    if (currentCooldown > 0) {
        return; 
    }

    CombatManager cm;

    if (cm.isInAttackRange(position, player.getPosition(), attackRange)) {
        int damageDealt = damage / 2;
        player.takeDamage(damageDealt);
        currentCooldown = attackCooldown;
        std::cout << "Enemy Tower attacked you! Lost " << damageDealt << " HP." << std::endl;
        return;
    }
}

CellType EnemyTower::getCellType() const {
    return CellType::ENEMY_TOWER;
}