#include "EnemyBuilding.h"
#include "Game.h"
#include "Enemy.h"
#include <iostream>
#include <sstream>

EnemyBuilding::EnemyBuilding(int x, int y, int health, int spawn_rate)
: Entity(x, y, health, 0),
spawn_rate_(spawn_rate),
turns_until_spawn_(spawn_rate) {}

void EnemyBuilding::update(Game& game) {
    if (!isAlive()) return;
    tick();
    if (shouldSpawn()) {
        Point spawn_base_pos = getPosition();
        bool spawned = false;
        for (int dx = -1; dx <= 1 && !spawned; ++dx) {
            for (int dy = -1; dy <= 1 && !spawned; ++dy) {
                if (dx == 0 && dy == 0) continue;
                    Point spawn_pos = {spawn_base_pos.x + dx, spawn_base_pos.y + dy};
                if (game.isCellFree(spawn_pos)) {
                    game.notifyLog("A new enemy has spawned near a building!");
                    game.getEntityManager().addEnemy(std::make_unique<Enemy>(spawn_pos.x, spawn_pos.y, 50, 5));
                    spawned = true;
                }
            }
        }
    }
}

bool EnemyBuilding::shouldSpawn() {
    if (turns_until_spawn_ <= 0) {
        turns_until_spawn_ = spawn_rate_;
        return true;
    }
    return false;
}

void EnemyBuilding::tick() {
    if (turns_until_spawn_ > 0) {
        turns_until_spawn_--;
    }
}

void EnemyBuilding::save(std::ostream& os) const {
    Entity::save(os);
    os << spawn_rate_ << " " << turns_until_spawn_ << " ";
}

void EnemyBuilding::load(std::istream& is) {
    Entity::load(is);
    is >> spawn_rate_ >> turns_until_spawn_;
}