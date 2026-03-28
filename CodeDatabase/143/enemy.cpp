#include "enemy.h"
#include <cmath>
#include <fstream>

Enemy::Enemy(const Position& position)
    : GameObject(position,
        GameConstants::INITIAL_ENEMY_HEALTH,
        GameConstants::INITIAL_ENEMY_DAMAGE) {
}

bool Enemy::canAttackPlayer(const Position& playerPosition) const {
    Position enemyPos = getPosition();
    int dx = std::abs(playerPosition.getX() - enemyPos.getX());
    int dy = std::abs(playerPosition.getY() - enemyPos.getY());
    return (dx == 1 && dy == 0) || (dx == 0 && dy == 1);
}