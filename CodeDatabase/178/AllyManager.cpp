#include "AllyManager.h"
#include <iostream>
#include <cmath>
#include <algorithm>

AllyManager::AllyManager(GameField& gameField) : field(gameField) {}

void AllyManager::updateAllies(std::vector<Ally> &allies, std::vector<Enemy> &enemies, Player &player) {
    for (auto &ally : allies) {
        if (!ally.isAlive()) continue;
        
        Enemy *closestEnemy = nullptr;
        int closestDist = 9999;
        
        for (auto &enemy : enemies) {
            if (!enemy.isAlive()) continue;
            int dist = std::abs(enemy.getPosition().x - ally.getPosition().x) +
                      std::abs(enemy.getPosition().y - ally.getPosition().y);
            if (dist < closestDist) {
                closestDist = dist;
                closestEnemy = &enemy;
            }
        }
        
        if (!closestEnemy) continue;
        
        if (closestDist <= 1) {
            closestEnemy->takeDamage(ally.getDamage());
            std::cout << "Ally attacked enemy at (" << closestEnemy->getPosition().x
                      << "," << closestEnemy->getPosition().y << ") for " << ally.getDamage() << " damage!\n";
                      
            if (!closestEnemy->isAlive()) {
                std::cout << "Enemy killed by ally! +10 coins\n";
                player.addCoins(10);
                player.addScore(10);
            }
        } else {
            Position allyPos = ally.getPosition();
            Position newPos = allyPos;
            
            if (std::abs(closestEnemy->getPosition().x - allyPos.x) >
                std::abs(closestEnemy->getPosition().y - allyPos.y)) {
                newPos.x += (closestEnemy->getPosition().x > allyPos.x) ? 1 : -1;
            } else {
                newPos.y += (closestEnemy->getPosition().y > allyPos.y) ? 1 : -1;
            }
            
            if (field.isValidPosition(newPos) && field.isPositionPassable(newPos)) {
                ally.move(newPos);
            }
        }
    }
    
    allies.erase(std::remove_if(allies.begin(), allies.end(),
                [](const Ally &a){ return !a.isAlive(); }), allies.end());
}

void AllyManager::addAlly(std::vector<Ally> &allies, const Ally &ally) {
    allies.push_back(ally);
}