#include "enemy_spawner.h"

#include <utility>


EnemySpawner::EnemySpawner(std::string name, const Position& position, int health, int spawn_interval, int cnt_spawned) :
    spawn_interval(spawn_interval), current_cooldown(spawn_interval), cnt_spawned(cnt_spawned),
    Entity(std::move(name), position, health, 0) {}


void EnemySpawner::decrease_cooldown() {
    if (current_cooldown > 0) current_cooldown--;
}

bool EnemySpawner::should_spawn() const {
    return current_cooldown == 0;
}

Enemy EnemySpawner::spawn_enemy() {
    current_cooldown = spawn_interval;
    cnt_spawned++;
    return {"Spawned_Enemy" + std::to_string(cnt_spawned), position, 30, 8};
}

int EnemySpawner::get_max_cooldown() const {
    return spawn_interval;
}

int EnemySpawner::get_spawn_cooldown() const {
    return current_cooldown;
}


