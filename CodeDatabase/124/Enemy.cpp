#include "enemy.h"
#include "player.h"
#include "gamefield.h"
#include <iostream>
#include <cmath>

Enemy::Enemy(int enemyLevel) : level(enemyLevel), isActive(true) {
    baseHealth = 40 + level * 10;
    baseDamage = 8 + level * 5;
    health = baseHealth;
    damage = baseDamage;
}

int Enemy::getHealth() const { return health; }
int Enemy::getDamage() const { return damage; }
bool Enemy::getIsActive() const { return isActive; }
bool Enemy::isAlive() const { return health > 0; }
int Enemy::getLevel() const { return level; }

void Enemy::takeDamage(int d) { 
    health -= d; 
    if (health < 0) health = 0; 
}

void Enemy::setActive(bool active) { 
    isActive = active; 
}

void Enemy::setLevel(int newLevel) {
    level = newLevel;
    baseHealth = 40 + level * 10;
    baseDamage = 8 + level * 5;
    health = baseHealth;
    damage = baseDamage;
}

bool Enemy::tryAttackPlayer(const GameField& field, int px, int py, Player& p) {
    int enemyX, enemyY;
    if (!field.findEnemyPosition(enemyX, enemyY)) return false;
    
    int dirs[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
    for (int i = 0; i < 4; i++) {
        if (enemyX + dirs[i][0] == px && enemyY + dirs[i][1] == py) {
            p.takeDamage(damage);
            std::cout << "Враг атаковал! " << damage << " урона!\n";
            return true;
        }
    }
    return false;
}

void Enemy::moveTowardsPlayer(GameField& field, Player& player) {
    if (!isActive) { 
        setActive(true);
        std::cout << "Враг оттаивает!\n";
        return;   
    }
     
    if (!isAlive()) return;   
    
    int playerX, playerY, enemyX, enemyY;
    if (!field.findPlayerPosition(playerX, playerY) || !field.findEnemyPosition(enemyX, enemyY)) {
        return;
    }
    
    if (tryAttackPlayer(field, playerX, playerY, player)) return;
    
    int dirs[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
    int bestX = enemyX, bestY = enemyY, bestDist = abs(playerX - enemyX) + abs(playerY - enemyY);
    
    for (int i = 0; i < 4; i++) {
        int nx = enemyX + dirs[i][0], ny = enemyY + dirs[i][1];
        if (field.isValidPosition(nx, ny) && field.getCell(nx, ny).getIsPassable() && 
            !(nx == playerX && ny == playerY)) {
            int nd = abs(playerX - nx) + abs(playerY - ny);
            if (nd < bestDist) { bestX = nx; bestY = ny; bestDist = nd; }
        }
    }
    
    if (bestX != enemyX || bestY != enemyY) {
        field.moveObject(enemyX, enemyY, bestX, bestY);
    }
}

void Enemy::setHealth(int newHealth) {
    health = newHealth;
    if (health < 0) health = 0;
}