#include "enemytower.h"
#include "player.h"
#include "gamefield.h"
#include <iostream>
#include <cmath>

EnemyTower::EnemyTower(int towerHealth, int towerDamage, int range)
    : health(towerHealth), damage(towerDamage), attackRange(range), 
      cooldown(2), currentCooldown(0), x(-1), y(-1), isAlive(true) {}

int EnemyTower::getHealth() const { return health; }
int EnemyTower::getDamage() const { return damage; }
int EnemyTower::getAttackRange() const { return attackRange; }
bool EnemyTower::getIsAlive() const { return isAlive; }
void EnemyTower::getPosition(int& outX, int& outY) const { outX = x; outY = y; }

void EnemyTower::place(int posX, int posY) {
    x = posX;
    y = posY;
}

void EnemyTower::takeDamage(int dmg) {
    health -= dmg;
    if (health <= 0) {
        health = 0;
        isAlive = false;
        std::cout << "Вражеская башня разрушена!\n";
    }
}

bool EnemyTower::canAttackPlayer(const GameField& field, const Player& player) const {
    if (!isAlive || currentCooldown > 0) return false;
    
    int playerX, playerY;
    if (!field.findPlayerPosition(playerX, playerY)) return false;
    
    int distance = std::max(std::abs(playerX - x), std::abs(playerY - y));
    return distance <= attackRange;
}

void EnemyTower::attackPlayer(Player& player) {
    player.takeDamage(damage);
    std::cout << "Вражеская башня атакует магией! " << damage << " урона!\n";
    currentCooldown = cooldown;
}

void EnemyTower::update(GameField& field, Player& player) {
    if (!isAlive) return;
    
    if (currentCooldown > 0) {
        currentCooldown--;
    }
    
    if (canAttackPlayer(field, player)) {
        attackPlayer(player);
    }
}