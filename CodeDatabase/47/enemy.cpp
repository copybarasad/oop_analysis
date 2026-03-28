#include "enemy.h"
#include "game_field.h"
#include <cmath>

Enemy::Enemy(int startX, int startY) {
    health = START_HEALTH;
    damage = START_DAMAGE;
    positionX = startX;
    positionY = startY;
}

int Enemy::getHealth() const {
    return health;
}

void Enemy::setHealth(int h) {
    health = h;
    if (health < 0) {
        health = 0;
    }
}

void Enemy::setDamage(int d) {
    damage = d;
    if (damage < 0) {
        damage = 0;
    }
}

int Enemy::getDamage() const {
    return damage;
}

int Enemy::getPositionX() const {
    return positionX;
}

int Enemy::getPositionY() const {
    return positionY;
}

void Enemy::setPosition(int x, int y) {
    positionX = x;
    positionY = y;
}

void Enemy::takeDamage(int damageAmount) {
    health -= damageAmount;
    if (health < 0) {
        health = 0;
    }
}

bool Enemy::isAlive() const {
    return health > 0;
}

bool Enemy::moveTowardsPlayer(int playerX, int playerY, const GameField& field, const std::vector<Enemy>& allEnemies, const EnemyBuilding& building, const EnemyTower& tower) {
    int deltaX = playerX - positionX;
    int deltaY = playerY - positionY;
    
    int absDeltaX = std::abs(deltaX);
    int absDeltaY = std::abs(deltaY);
    
    auto isCellOccupied = [&](int x, int y) {
        for (const auto& otherEnemy : allEnemies) {
            if (&otherEnemy != this && otherEnemy.isAlive() && otherEnemy.getPositionX() == x && otherEnemy.getPositionY() == y) {
                return true;
            }
        }
        if (building.isAlive() && building.getPositionX() == x && building.getPositionY() == y) {
            return true;
        }
        if (tower.isAlive() && tower.getPositionX() == x && tower.getPositionY() == y) {
            return true;
        }
        return false;
    };
    
    if (absDeltaX >= absDeltaY && deltaX != 0) {
        int directionX = (deltaX > 0) ? 1 : -1;
        int newX = positionX + directionX;
        int newY = positionY;
        
        if (newX == playerX && newY == playerY) {
            return true;
        }
        
        if (field.isValidPosition(newX, newY) && field.isPassable(newX, newY) && !isCellOccupied(newX, newY)) {
            positionX = newX;
            return false;
        }
    }
    
    if (deltaY != 0) {
        int directionY = (deltaY > 0) ? 1 : -1;
        int newX = positionX;
        int newY = positionY + directionY;
        
        if (newX == playerX && newY == playerY) {
            return true;
        }
        
        if (field.isValidPosition(newX, newY) && field.isPassable(newX, newY) && !isCellOccupied(newX, newY)) {
            positionY = newY;
            return false;
        }
    }
    
    if (absDeltaX < absDeltaY && deltaX != 0) {
        int directionX = (deltaX > 0) ? 1 : -1;
        int newX = positionX + directionX;
        int newY = positionY;
        
        if (newX == playerX && newY == playerY) {
            return true;
        }
        
        if (field.isValidPosition(newX, newY) && field.isPassable(newX, newY) && !isCellOccupied(newX, newY)) {
            positionX = newX;
            return false;
        }
    }
    
    if (deltaX != 0 && deltaY != 0) {
        int directionX = (deltaX > 0) ? 1 : -1;
        int directionY = (deltaY > 0) ? 1 : -1;
        int newX = positionX + directionX;
        int newY = positionY + directionY;
        
        if (newX == playerX && newY == playerY) {
            return true;
        }
        
        if (field.isValidPosition(newX, newY) && field.isPassable(newX, newY) && !isCellOccupied(newX, newY)) {
            positionX = newX;
            positionY = newY;
            return false;
        }
    }
    
    if (deltaX != 0 && deltaY != 0) {
        int directionX = (deltaX > 0) ? 1 : -1;
        int directionY = (deltaY > 0) ? -1 : 1;
        int newX = positionX + directionX;
        int newY = positionY + directionY;
        
        if (newX == playerX && newY == playerY) {
            return true;
        }
        
        if (field.isValidPosition(newX, newY) && field.isPassable(newX, newY) && !isCellOccupied(newX, newY)) {
            positionX = newX;
            positionY = newY;
            return false;
        }
    }

    if (deltaX != 0 && deltaY != 0) {
        int directionX = (deltaX > 0) ? -1 : 1;
        int directionY = (deltaY > 0) ? 1 : -1;
        int newX = positionX + directionX;
        int newY = positionY + directionY;
        
        if (newX == playerX && newY == playerY) {
            return true;
        }
        
        if (field.isValidPosition(newX, newY) && field.isPassable(newX, newY) && !isCellOccupied(newX, newY)) {
            positionX = newX;
            positionY = newY;
            return false;
        }
    }
    
    return false;
}