#include "Enemy.h"
#include "Player.h"
#include "GameField.h"
#include <stdexcept>
#include <cmath>
#include <random>
#include <iostream>
#include <sstream>
#include <string>

Enemy::Enemy(int startX, int startY, int initialHealth, 
             int enemyDamage, int enemyId)
    : health(initialHealth), maxHealth(initialHealth), 
      damage(enemyDamage), positionX(startX), positionY(startY), 
      id(enemyId) {
    
    if (initialHealth <= 0 || enemyDamage <= 0) {
        throw std::invalid_argument("Health and damage values must be positive");
    }
    
    validateInvariant();
}

void Enemy::validateInvariant() const {
    if (health < 0 || health > maxHealth || damage <= 0) {
        throw std::runtime_error("Enemy invariant violated");
    }
}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getMaxHealth() const {
    return maxHealth;
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

int Enemy::getId() const {
    return id;
}

bool Enemy::isAlive() const {
    return health > 0;
}

bool Enemy::moveTo(int newX, int newY) {
    positionX = newX;
    positionY = newY;
    return true;
}

void Enemy::takeDamage(int damageAmount) {
    if (damageAmount < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    
    health -= damageAmount;
    if (health < 0) {
        health = 0;
    }
    
    validateInvariant();
}

void Enemy::attackPlayer(Player& player) {
    if (!isAlive()) {
        return;
    }
    
    player.takeDamage(damage);
}

int Enemy::calculateDistanceToPlayer(const Player& player) const {
    int dx = positionX - player.getPositionX();
    int dy = positionY - player.getPositionY();
    return static_cast<int>(std::sqrt(dx * dx + dy * dy));
}

void Enemy::makeMove(GameField& field, const Player& player) {
    if (!isAlive()) {
        return;
    }
    
    int playerX = player.getPositionX();
    int playerY = player.getPositionY();
    
    // Проверяем, можем ли мы атаковать игрока или союзника (melee атака - только соседние клетки, не по диагонали)
    int dxToPlayer = std::abs(positionX - playerX);
    int dyToPlayer = std::abs(positionY - playerY);
    
    // Melee атака игрока: только если на соседней клетке и не по диагонали
    if ((dxToPlayer == 1 && dyToPlayer == 0) || (dxToPlayer == 0 && dyToPlayer == 1)) {
        // Атакуем игрока
        const_cast<Player&>(player).takeDamage(damage);
        std::cout << "Enemy attacked player for " << damage << " damage!" << std::endl;
        return; // Не двигаемся, только атакуем
    }
    
    // Проверяем союзников на соседних клетках
    // Получаем доступ к союзникам через GameField (нужно добавить метод в Game)
    // Пока пропускаем атаку союзников, так как нужен доступ к Game
    
    // Движение только горизонтально или вертикально (не по диагонали)
    // Приоритет: сначала по X, потом по Y
    int dx = 0;
    int dy = 0;
    
    // Сначала пытаемся двигаться по X
    if (positionX < playerX) {
        dx = 1;
        dy = 0;
    } else if (positionX > playerX) {
        dx = -1;
        dy = 0;
    } else {
        // Если X одинаковый, двигаемся по Y
        if (positionY < playerY) {
            dx = 0;
            dy = 1;
        } else if (positionY > playerY) {
            dx = 0;
            dy = -1;
        }
    }
    
    int newX = positionX + dx;
    int newY = positionY + dy;
    
    // Проверяем, что можем двигаться на новую позицию (не через стены)
    if (field.canMoveTo(newX, newY)) {
        // Clear current position
        field.clearPosition(positionX, positionY);
        
        // Move to new position
        moveTo(newX, newY);
        field.setEnemyPosition(newX, newY);
    } else {
        // Если не можем двигаться по приоритетному направлению, пробуем альтернативное
        // Если пытались двигаться по X, пробуем по Y
        if (dx != 0) {
            dx = 0;
            if (positionY < playerY) {
                dy = 1;
            } else if (positionY > playerY) {
                dy = -1;
            }
            
            newX = positionX + dx;
            newY = positionY + dy;
            
            if (field.canMoveTo(newX, newY)) {
                field.clearPosition(positionX, positionY);
                moveTo(newX, newY);
                field.setEnemyPosition(newX, newY);
            }
        } else if (dy != 0) {
            // Если пытались двигаться по Y, пробуем по X
            dy = 0;
            if (positionX < playerX) {
                dx = 1;
            } else if (positionX > playerX) {
                dx = -1;
            }
            
            newX = positionX + dx;
            newY = positionY + dy;
            
            if (field.canMoveTo(newX, newY)) {
                field.clearPosition(positionX, positionY);
                moveTo(newX, newY);
                field.setEnemyPosition(newX, newY);
            }
        }
    }
}

void Enemy::heal(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Heal amount cannot be negative");
    }
    
    health += amount;
    if (health > maxHealth) {
        health = maxHealth;
    }
    
    validateInvariant();
}

std::string Enemy::serialize() const {
    std::ostringstream oss;
    // Формат: health:maxHealth:damage:positionX:positionY:id
    oss << health << ":" << maxHealth << ":" << damage << ":"
        << positionX << ":" << positionY << ":" << id << "\n";
    return oss.str();
}

bool Enemy::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    
    // health
    if (!std::getline(iss, token, ':')) return false;
    health = std::stoi(token);
    
    // maxHealth
    if (!std::getline(iss, token, ':')) return false;
    maxHealth = std::stoi(token);
    
    // damage
    if (!std::getline(iss, token, ':')) return false;
    damage = std::stoi(token);
    
    // positionX
    if (!std::getline(iss, token, ':')) return false;
    positionX = std::stoi(token);
    
    // positionY
    if (!std::getline(iss, token, ':')) return false;
    positionY = std::stoi(token);
    
    // id
    if (!std::getline(iss, token)) return false;
    if (!token.empty() && token.back() == '\n') {
        token.pop_back();
    }
    if (!token.empty() && token.back() == '\r') {
        token.pop_back();
    }
    id = std::stoi(token);
    
    validateInvariant();
    return true;
}

