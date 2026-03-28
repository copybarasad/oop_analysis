#include "building.h"

Building::Building(int health, int damage, int x, int y) : Enemy(health, damage, x, y) {}

Enemy* Building::SpawnEnemy() {
    return new Enemy(50, 10, GetX(), GetY());
}