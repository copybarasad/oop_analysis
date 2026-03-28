#include "TrapManager.h"
#include <algorithm>

void TrapManager::addTrap(std::unique_ptr<Trap> trap) {
    if (trap) {
        traps_.push_back(std::move(trap));
    }
}

bool TrapManager::hasTrapOnCell(int x, int y) const {
    for (const auto& trap : traps_) {
        if (trap) {
            Pos pos = trap->getPosition();
            if (pos.x == x && pos.y == y) {
                return true;
            }
        }
    }
    return false;
}

int TrapManager::activateTrap(int x, int y) {
    for (auto it = traps_.begin(); it != traps_.end(); ++it) {
        if (*it) {
            Pos pos = (*it)->getPosition();
            if (pos.x == x && pos.y == y) {
                int damage = (*it)->getDamage();
                traps_.erase(it);
                return damage;
            }
        }
    }
    return 0;
}

int TrapManager::getTrapCount() const {
    return static_cast<int>(traps_.size());
}

