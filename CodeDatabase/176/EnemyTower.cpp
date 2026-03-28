#include "EnemyTower.h"
#include "GameManager.h"
#include "Player.h"
#include <iostream>
#include <cmath>

EnemyTower::EnemyTower(int x, int y, int range)
    : posX(x), posY(y), attackRange(range), cooldown(2), currentCooldown(0) {}

void EnemyTower::update() {
    if (currentCooldown > 0) {
        currentCooldown--;
    }
}

bool EnemyTower::tryAttackPlayer(GameManager& gameManager) {
    if (currentCooldown > 0) {
        return false;
    }
    
    auto playerPos = gameManager.getPlayerPosition();
    int playerX = playerPos.first;
    int playerY = playerPos.second;
    
    bool playerInRange = false;
    for (int y = posY - 1; y <= posY + 1; ++y) {
        for (int x = posX - 1; x <= posX + 1; ++x) {
            if (x == playerX && y == playerY) {
                playerInRange = true;
                break;
            }
        }
        if (playerInRange) break;
    }
    
    if (playerInRange) {
        std::cout << "Enemy tower attacks player for 10 damage!\n";
        const_cast<Player&>(gameManager.getPlayer()).takeDamage(10);
        currentCooldown = cooldown;
        return true;
    }
    
    return false;
}

int EnemyTower::getPosX() const {
    return posX;
}

int EnemyTower::getPosY() const {
    return posY;
}

bool EnemyTower::isOnCooldown() const {
    return currentCooldown > 0;
}
