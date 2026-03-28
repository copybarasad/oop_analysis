#include "EnemyBuilding.h"

EnemyBuilding::EnemyBuilding(int x, int y, int spawn_Range, int health)
    : position(x, y), spawn_Counter(0), spawn_Range(spawn_Range), health(health) {
}

bool EnemyBuilding::should_SpawnEnemy() const {
    return spawn_Counter >= spawn_Range;
}

void EnemyBuilding::update() {
    spawn_Counter++;
}

void EnemyBuilding::reset_Counter() {
    spawn_Counter = 0;
}

void EnemyBuilding::take_Damage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool EnemyBuilding::isAlive() const {
    return health > 0;
}

int EnemyBuilding::get_Health() const {
    return health;
}

const Position& EnemyBuilding::get_Position() const {
    return position;
}

int EnemyBuilding::get_SpawnCounter() const {
    return spawn_Counter;
}

int EnemyBuilding::get_SpawnRange() const {
    return spawn_Range;
}

void EnemyBuilding::setHealth(int h) {
    health = h;
}

void EnemyBuilding::setSpawnCounter(int counter) {
    spawn_Counter = counter;
}
