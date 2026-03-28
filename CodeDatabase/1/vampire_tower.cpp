#include "vampire_tower.h"
#include <stdexcept>

VampireTower::VampireTower(const Position& position)
    : position_(position), health_(80), attackCooldown_(0)  
{}

void VampireTower::takeDamage(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    health_ -= amount;
    if (health_ < 0) {
        health_ = 0;
    }
}

void VampireTower::decreaseCooldown() {
    if (attackCooldown_ > 0) {
        attackCooldown_--;
    }
}

SpellCastResult VampireTower::performAttack(const Position& target) {
    if (attackCooldown_ == 0) {
        attackCooldown_ = 6;  
        
        SpellCastResult result;
        result.affectedPositions.push_back(target);
        result.damage = 5;  
        result.isTrap = false;
        return result;
    }
    return SpellCastResult();  
}

bool VampireTower::canAttack(const Position& hunterPosition) const {
    return position_.manhattanDistance(hunterPosition) <= 3;  
}

bool VampireTower::isReadyToAttack() const {
    return attackCooldown_ == 0;
}

Position VampireTower::getPosition() const {
    return position_;
}

int VampireTower::getHealth() const {
    return health_;
}
bool VampireTower::isAlive() const {
    return health_ > 0;
}

