#include "TowerManager.h"
#include "Player.h"
#include <algorithm>
#include <iostream>

void TowerManager::addTower(std::unique_ptr<EnemyTower> tower) {
    if (tower) {
        towers_.push_back(std::move(tower));
    }
}

int TowerManager::getTowerCount() const {
    return static_cast<int>(towers_.size());
}

void TowerManager::removeDestroyedTowers() {
    towers_.erase(
        std::remove_if(towers_.begin(), towers_.end(),
            [](const std::unique_ptr<EnemyTower>& tower) {
                return !tower->isAlive();
            }),
        towers_.end()
    );
}

bool TowerManager::hasTowerOnCell(int x, int y) const {
    for (const auto& tower : towers_) {
        if (tower && tower->isAlive()) {
            Pos pos = tower->getPosition();
            if (pos.x == x && pos.y == y) {
                return true;
            }
        }
    }
    return false;
}

EnemyTower& TowerManager::getTowerAt(int x, int y) const {
    for (const auto& tower : towers_) {
        if (tower && tower->isAlive()) {
            Pos pos = tower->getPosition();
            if (pos.x == x && pos.y == y) {
                return *tower.get();
            }
        }
    }
    throw std::runtime_error("Tower not found at position");
}

void TowerManager::processTowerAttacks(Player& player) {
    if (!player.isAlive()) return;
    
    Pos playerPos = player.getPosition();
    
    for (auto& tower : towers_) {
        if (!tower || !tower->isAlive()) continue;
        
        tower->nextTurn();

        if (tower->canAttack() && tower->isInRange(playerPos)) {
            player.takeDamage(tower->getAttackDamage());
            tower->resetCooldown();
            Pos towerPos = tower->getPosition();
            std::cout << "Tower at (" << towerPos.x << "," << towerPos.y 
                     << ") attacks you for " << tower->getAttackDamage() << " damage!\n";
        }
    }
}

