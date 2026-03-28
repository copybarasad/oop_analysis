#include "EnemyBuilding.h"
#include <algorithm>

EnemyBuilding::EnemyBuilding(int x, int y, int spawn_interval, int health)
    : x_(x),
      y_(y),
      health_(health),
      max_health_(health),
      spawn_interval_(spawn_interval),
      turns_since_last_spawn_(0) {}

int EnemyBuilding::GetX() const {
    return x_;
}

int EnemyBuilding::GetY() const {
    return y_;
}

int EnemyBuilding::GetHealth() const {
    return health_;
}

int EnemyBuilding::GetMaxHealth() const {
    return max_health_;
}

int EnemyBuilding::GetSpawnInterval() const {
    return spawn_interval_;
}

int EnemyBuilding::GetTurnsSinceLastSpawn() const {
    return turns_since_last_spawn_;
}

bool EnemyBuilding::ShouldSpawnEnemy() {
    turns_since_last_spawn_++;
    if (turns_since_last_spawn_ >= spawn_interval_) {
        turns_since_last_spawn_ = 0;
        return true;
    }
    return false;
}

void EnemyBuilding::TakeDamage(int damage) {
    health_ = std::max(0, health_ - damage);
}

bool EnemyBuilding::IsAlive() const {
    return health_ > 0;
}

void EnemyBuilding::IncreaseStats(int health_bonus) {
    max_health_ += health_bonus;
    health_ += health_bonus;
}

void EnemyBuilding::SetTurnsSinceLastSpawn(int turns) {
    turns_since_last_spawn_ = (turns > 0) ? turns : 0;
}
