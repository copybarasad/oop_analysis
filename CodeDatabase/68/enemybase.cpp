#include "enemybase.h"
#include "gamefield.h"
#include <iostream>

EnemyBase::EnemyBase(Position pos, int health) 
    : position(pos), health(health), spawnCooldown(0), maxSpawnCooldown(3) {}

void EnemyBase::takeTurn(GameField* field, std::vector<MovingEnemy>& enemies) {
    if (spawnCooldown > 0) {
        spawnCooldown--;
        return;
    }
    
    // Spawn new enemy
    Position spawnPos(position.x + 1, position.y);
    if (field->isValidPosition(spawnPos) && !field->hasBuildingAt(spawnPos)) {
        enemies.emplace_back(spawnPos, 30, 5);
        std::cout << "Enemy base spawns new enemy!\n";
        spawnCooldown = maxSpawnCooldown;
    }
}

void EnemyBase::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        std::cout << "Enemy base destroyed!\n";
    }
}

bool EnemyBase::isAlive() const {
    return health > 0;
}

Position EnemyBase::getPosition() const {
    return position;
}