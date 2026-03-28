#include "lib/EnemyTower.h"
#include "lib/GameField.h"
#include "lib/Player.h"
#include <cmath>
#include <iostream>

EnemyTower::EnemyTower(GameField* gameField, int posX, int posY, int towerHealth, int damage, int range) 
    : field(gameField),
      health(towerHealth),
      maxHealth(towerHealth),
      x(posX),
      y(posY),
      attackRange(range),
      attackDamage(damage),
      canAttack(true),
      alive(true) {
    
    // Устанавливаем позицию башни на поле
    if (field && field->isValidPosition(x, y)) {
        field->setTowerPosition(x, y);
    }
}

int EnemyTower::getHealth() const {
    return health;
}

int EnemyTower::getMaxHealth() const {
    return maxHealth;
}

int EnemyTower::getX() const {
    return x;
}

int EnemyTower::getY() const {
    return y;
}

int EnemyTower::getAttackRange() const {
    return attackRange;
}

int EnemyTower::getAttackDamage() const {
    return attackDamage;
}

bool EnemyTower::canAttackThisTurn() const {
    return canAttack && alive;
}

bool EnemyTower::isAlive() const {
    return alive;
}

void EnemyTower::takeDamage(int damageAmount) {
    if (!alive || damageAmount <= 0) return;
    
    health -= damageAmount;
    std::cout << "Enemy Tower received " << damageAmount << " damage! Health: " << health << "/" << maxHealth << "\n";
    
    if (health <= 0) {
        health = 0;
        alive = false;
        // Очищаем позицию башни на поле
        if (field) {
            field->clearTowerPosition(x, y);
        }
        std::cout << "Enemy Tower destroyed!\n";
    }
}

void EnemyTower::attackPlayer(Player& player) {
    if (!alive || !canAttack || !player.isPlayerAlive()) return;
    
    double distance = std::sqrt(std::pow(player.getX() - x, 2) + std::pow(player.getY() - y, 2));
    if (distance <= attackRange) {
        std::cout << "Enemy Tower casts a weakened fire bolt!\n";
        player.takeDamage(attackDamage);
        canAttack = false; // Cannot attack next turn
    }
}

void EnemyTower::resetAttack() {
    canAttack = true;
}

void EnemyTower::update(Player& player) {
    if (alive) {
        attackPlayer(player);
    }
}