#include "EnemyBuilding.hpp"

EnemyBuilding::EnemyBuilding(int x, int y, int health, int cooldown)
    : Building(x, y, health), cooldown_(cooldown), spawnCounter_(0)
{
    if (cooldown_ <= 0) {
        throw std::invalid_argument("Cooldown must be positive");
    }
}
    
void EnemyBuilding::update() {
    if (isDestroyed()) {
        return;
    }

    if (spawnCounter_ < cooldown_){
       spawnCounter_++;
    }
}

bool EnemyBuilding::isActive() const {
    return !isDestroyed();
}

int EnemyBuilding::getCooldown() const { 
    return cooldown_; 
}

bool EnemyBuilding::canSpawn() const {
    return spawnCounter_ >= cooldown_;
}
    
void EnemyBuilding::resetCounter() {
    spawnCounter_ = 0;
}