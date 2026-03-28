#include "../include/TrapManager.h"
#include "../include/EnemyManager.h"

void TrapManager::addTrap(const Position& pos, std::unique_ptr<Trap> trap) {
    traps.push_back({pos, std::move(trap)});
}

bool TrapManager::hasTrapAt(const Position& pos) const {
    for (const auto& trapPair : traps) {
        if (trapPair.first == pos) {
            return true;
        }
    }
    return false;
}

int TrapManager::triggerTrap(const Position& pos, EnemyManager& enemyManager) {
    for (auto it = traps.begin(); it != traps.end(); ++it) {
        if (it->first == pos) {
            int damage = it->second->getDamage();
            enemyManager.attackEnemyAt(pos, damage);
            traps.erase(it);
            return damage;
        }
    }
    return 0;
}

const std::vector<std::pair<Position, std::unique_ptr<Trap>>>& TrapManager::getTraps() const {
    return traps;
}
