#include "EnemyBuilding.h"

EnemyBuilding::EnemyBuilding(int x, int y, int spawn_interval)
        : Entity(x, y),
          spawn_interval_(spawn_interval),
          spawn_timer_(spawn_interval) {}

bool EnemyBuilding::update() {
    if (spawn_timer_ > 0) {
        --spawn_timer_;
        return false;
    }
    spawn_timer_ = spawn_interval_;
    return true;
}
