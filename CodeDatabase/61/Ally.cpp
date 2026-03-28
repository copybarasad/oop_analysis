#include "Ally.h"
#include "Player.h"
#include "GameField.h"
#include "Enemy.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

Ally::Ally(int startX, int startY, int initialHealth, int allyDamage, int allyId)
    : health(initialHealth), maxHealth(initialHealth), damage(allyDamage),
      positionX(startX), positionY(startY), id(allyId), alive(true) {
    
    if (initialHealth <= 0 || allyDamage <= 0) {
        throw std::invalid_argument("Health and damage values must be positive");
    }
}

int Ally::getHealth() const {
    return health;
}

int Ally::getMaxHealth() const {
    return maxHealth;
}

int Ally::getDamage() const {
    return damage;
}

int Ally::getPositionX() const {
    return positionX;
}

int Ally::getPositionY() const {
    return positionY;
}

int Ally::getId() const {
    return id;
}

bool Ally::isAlive() const {
    return alive && health > 0;
}

bool Ally::moveTo(int newX, int newY) {
    positionX = newX;
    positionY = newY;
    return true;
}

void Ally::takeDamage(int damageAmount) {
    if (damageAmount < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    
    health -= damageAmount;
    if (health <= 0) {
        health = 0;
        alive = false;
    }
}

void Ally::attackEnemy(Enemy& enemy) {
    if (!isAlive()) {
        return;
    }
    
    enemy.takeDamage(damage);
}

int Ally::calculateDistanceToPlayer(const Player& player) const {
    int dx = positionX - player.getPositionX();
    int dy = positionY - player.getPositionY();
    return static_cast<int>(std::sqrt(dx * dx + dy * dy));
}

void Ally::makeMove(GameField& field, const Player& player, std::vector<Enemy>& enemies) {
    if (!isAlive()) {
        return;
    }
    
    int playerX = player.getPositionX();
    int playerY = player.getPositionY();
    
    // Ищем ближайшего врага
    Enemy* nearestEnemy = nullptr;
    int minDistance = 1000;
    
    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;
        
        int dx = enemy.getPositionX() - positionX;
        int dy = enemy.getPositionY() - positionY;
        int distance = static_cast<int>(std::sqrt(dx * dx + dy * dy));
        
        if (distance < minDistance) {
            minDistance = distance;
            nearestEnemy = &enemy;
        }
    }
    
    // Если враг рядом (соседняя клетка, не по диагонали), атакуем
    if (nearestEnemy) {
        int dxToEnemy = std::abs(nearestEnemy->getPositionX() - positionX);
        int dyToEnemy = std::abs(nearestEnemy->getPositionY() - positionY);
        
        // Melee атака: только если на соседней клетке и не по диагонали
        if ((dxToEnemy == 1 && dyToEnemy == 0) || (dxToEnemy == 0 && dyToEnemy == 1)) {
            attackEnemy(*nearestEnemy);
            std::cout << "Ally attacked enemy for " << damage << " damage!" << std::endl;
            return;
        }
    }
    
    // Двигаемся к ближайшему врагу
    if (nearestEnemy) {
        int targetX = nearestEnemy->getPositionX();
        int targetY = nearestEnemy->getPositionY();
        
        int dx = 0;
        int dy = 0;
        
        if (positionX < targetX) dx = 1;
        else if (positionX > targetX) dx = -1;
        
        if (positionY < targetY) dy = 1;
        else if (positionY > targetY) dy = -1;
        
        int newX = positionX + dx;
        int newY = positionY + dy;
        
        if (field.canMoveTo(newX, newY) && !field.getCell(newX, newY).hasPlayerOnCell()) {
            field.clearPosition(positionX, positionY);
            moveTo(newX, newY);
            field.setAllyPosition(newX, newY);
        }
    }
}

void Ally::heal(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Heal amount cannot be negative");
    }
    
    health += amount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

std::string Ally::serialize() const {
    std::ostringstream oss;
    // Формат: health:maxHealth:damage:positionX:positionY:id:alive
    oss << health << ":" << maxHealth << ":" << damage << ":"
        << positionX << ":" << positionY << ":" << id << ":"
        << (alive ? 1 : 0) << "\n";
    return oss.str();
}

bool Ally::deserialize(const std::string& data) {
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
    if (!std::getline(iss, token, ':')) return false;
    id = std::stoi(token);
    
    // alive
    if (!std::getline(iss, token)) return false;
    if (!token.empty() && token.back() == '\n') {
        token.pop_back();
    }
    if (!token.empty() && token.back() == '\r') {
        token.pop_back();
    }
    alive = (std::stoi(token) == 1);
    
    return true;
}

