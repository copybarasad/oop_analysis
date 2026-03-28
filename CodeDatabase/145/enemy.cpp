#include "enemy.h"
#include <cstdlib>
#include <ctime>

Enemy::Enemy(int health, int damage, int x, int y) 
    : GameEntity(health, damage), position(x, y) {
    srand(static_cast<unsigned>(time(nullptr)) + x + y);
}

std::pair<int, int> Enemy::getPosition() const {
    return position;
}

void Enemy::setPosition(int x, int y) {
    position = {x, y};
}

std::pair<int, int> Enemy::calculateMoveTowardsPlayer(const std::pair<int, int>& playerPos) const {
    int dx = 0, dy = 0;
    
    if (position.first < playerPos.first) dx = 1;
    else if (position.first > playerPos.first) dx = -1;
    
    if (position.second < playerPos.second) dy = 1;
    else if (position.second > playerPos.second) dy = -1;
    
    if (dx != 0 && dy != 0) {
        if (rand() % 2 == 0) dx = 0;
        else dy = 0;
    }
    
    return {position.first + dx, position.second + dy};
}