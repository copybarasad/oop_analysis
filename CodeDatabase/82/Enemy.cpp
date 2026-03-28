#include "Enemy.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

Enemy::Enemy(int startX, int startY) : position(startX, startY) {}

void Enemy::move(const Position& playerPosition, const std::vector<std::vector<bool>>& passableMap,
                 const std::vector<Enemy>& otherEnemies) { 
    static bool initialized = false;
    if (!initialized) {
        std::srand(std::time(nullptr));
        initialized = true;
    }
    
    int preferedX = (playerPosition.x > position.x) ? 1 : -1;
    int preferedY = (playerPosition.y > position.y) ? 1 : -1;

    std::vector<std::pair<int, int>> moves;

    if (std::abs(playerPosition.x - position.x) > std::abs(playerPosition.y - position.y)) {
        moves.push_back({preferedX, 0});
        moves.push_back({0, preferedY});
        moves.push_back({0, -preferedY});
        moves.push_back({-preferedX, 0});
    } else {
        moves.push_back({0, preferedY});
        moves.push_back({preferedX, 0});
        moves.push_back({-preferedX, 0});
        moves.push_back({0, -preferedY});
    }

    for (const auto& move : moves) {
        int newX = position.x + move.first;
        int newY = position.y + move.second;

        if (tryMode(newX, newY, passableMap, otherEnemies)) {
            return; 
        }
    }
}

bool Enemy::tryMode(int newX, int newY, const std::vector<std::vector<bool>>& passableMap,
                    const std::vector<Enemy>& otherEnemies) {
    if (newX < 0 || newX >= static_cast<int>(passableMap[0].size()) || 
        newY < 0 || newY >= static_cast<int>(passableMap.size())) {
        return false;
    }
    
    if (!passableMap[newY][newX]) {
        return false;
    }
    
    for (const auto& otherEnemy : otherEnemies) {
        if (&otherEnemy != this && otherEnemy.isAlive() && 
            otherEnemy.getPosition().x == newX && otherEnemy.getPosition().y == newY) {
            return false;
        }
    }
    
    position.x = newX;
    position.y = newY;
    return true;
}

void Enemy::takeDamage(int damageAmount) {
    health -= damageAmount;
}