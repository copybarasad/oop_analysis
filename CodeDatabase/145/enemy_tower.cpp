#include "enemy_tower.h"
#include <cmath>

EnemyTower::EnemyTower(int x, int y, int attackRadius, int spellDamage, int cooldown)
    : GameEntity(40, 0),  
      position(x, y),
      attackRadius(attackRadius),
      spellDamage(spellDamage),
      cooldownCounter(0),
      cooldownDuration(cooldown) {}

std::pair<int, int> EnemyTower::getPosition() const {
    return position;
}

void EnemyTower::setPosition(int x, int y) {
    position = {x, y};
}

bool EnemyTower::isPlayerInRange(const std::pair<int, int>& playerPos) const {
    
    int dx = playerPos.first - position.first;
    int dy = playerPos.second - position.second;
    double distance = std::sqrt(dx * dx + dy * dy);
    
    return distance <= attackRadius;
}

bool EnemyTower::canAttack() const {
    return cooldownCounter == 0;
}

int EnemyTower::getSpellDamage() const {
    return spellDamage;
}

int EnemyTower::getAttackRadius() const {
    return attackRadius;
}

void EnemyTower::update() {
    
    if (cooldownCounter > 0) {
        cooldownCounter--;
    }
}

void EnemyTower::performAttack() {
    
    cooldownCounter = cooldownDuration;
}

int EnemyTower::getScore() const {
    return 200;  
}

int EnemyTower::getCooldownCounter() const {
    return cooldownCounter;
}

void EnemyTower::setCooldownCounter(int c) {
    cooldownCounter = c;
}


