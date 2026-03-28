#include "lib/Enemy.h"
#include "lib/Player.h"
#include "lib/GameField.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Конструктор
Enemy::Enemy(GameField* gameField, int startX, int startY, int enemyHealth, int enemyDamage) 
    : field(gameField) {
    health = enemyHealth;
    maxHealth = enemyHealth;
    damage = enemyDamage;
    x = startX;
    y = startY;
    alive = true;
    symbol = 'E';
    
    srand(time(0));
    
    // Устанавливаем позицию на поле
    if (field && field->isValidPosition(x, y)) {
        field->setEnemyPosition(x, y);
    }
}

// Геттеры
int Enemy::getHealth() const {
    return health;
}

int Enemy::getMaxHealth() const {
    return maxHealth;
}

int Enemy::getDamage() const {
    return damage;
}

int Enemy::getX() const {
    return x;
}

int Enemy::getY() const {
    return y;
}

bool Enemy::isAlive() const {
    return alive;
}

char Enemy::getSymbol() const {
    return symbol;
}

// Сеттеры
void Enemy::setPosition(int newX, int newY) {
    if (alive && field && field->isValidPosition(newX, newY) && field->isCellPassable(newX, newY)) {
        // Очищаем старую позицию
        if (field->isValidPosition(x, y)) {
            field->clearCell(x, y);
        }
        
        x = newX;
        y = newY;
        field->setEnemyPosition(x, y);
    }
}

// Проверка валидности перемещения
bool Enemy::isValidMove(int newX, int newY) const {
    return field && field->isValidPosition(newX, newY) && 
           field->isCellPassable(newX, newY) && 
           !field->getCell(newX, newY).getHasPlayer();
}

// Движение к игроку
void Enemy::moveTowardsPlayer(int playerX, int playerY, Player& player) {
    if (!alive || !player.isPlayerAlive() || !field) return;
    
    // Очищаем текущую позицию
    field->clearCell(x, y);
    
    // Проверяем, находится ли враг на соседней клетке с игроком
    int distanceX = std::abs(playerX - x);
    int distanceY = std::abs(playerY - y);
    
    // Если враг рядом с игроком (расстояние 1 клетка)
    if (distanceX <= 1 && distanceY <= 1 && (distanceX == 1 || distanceY == 1)) {
        std::cout << "Enemy attacks player!\n";
        player.takeDamage(damage);
        // Возвращаем врага на текущую позицию
        field->setEnemyPosition(x, y);
        return;
    }
    
    // Движение к игроку
    int dx = 0, dy = 0;
    
    if (playerX > x) dx = 1;
    else if (playerX < x) dx = -1;
    
    if (playerY > y) dy = 1;
    else if (playerY < y) dy = -1;
    
    // Иногда враг движется случайно (для реалистичности)
    if (rand() % 4 == 0) { // 25% chance random move
        dx = (rand() % 3) - 1; // -1, 0, or 1
        dy = (rand() % 3) - 1;
    }
    
    // Проверяем, не пытается ли враг встать на клетку игрока
    if (x + dx == playerX && y + dy == playerY) {
        std::cout << "Enemy attacks player from adjacent cell!\n";
        player.takeDamage(damage);
        // Возвращаем врага на текущую позицию
        field->setEnemyPosition(x, y);
    } else if (isValidMove(x + dx, y + dy)) {
        x += dx;
        y += dy;
        field->setEnemyPosition(x, y);
        std::cout << "Enemy moves to position (" << x << ", " << y << ")\n";
    } else {
        // Если движение невозможно, возвращаем врага на текущую позицию
        field->setEnemyPosition(x, y);
        std::cout << "Enemy cannot move to blocked position\n";
    }
}

// Получение урона
void Enemy::takeDamage(int damageAmount) {
    if (!alive || damageAmount <= 0) return;
    
    std::cout << "DEBUG: Enemy taking " << damageAmount << " damage. Health was: " << health << "\n";
    health -= damageAmount;
    std::cout << "DEBUG: Enemy health now: " << health << "\n";
    
    std::cout << "Enemy received " << damageAmount << " damage! Health: " << health << "/" << maxHealth << "\n";
    
    if (health <= 0) {
        std::cout << "DEBUG: Enemy defeated!\n";
        health = 0;
        alive = false;
        if (field) {
            field->clearCell(x, y);
        }
        std::cout << "Enemy defeated! +10 score\n";
    }
}

// Отображение статуса
void Enemy::displayStatus() const {
    std::cout << "\n=== Enemy Status ===\n";
    std::cout << "Health: " << health << "/" << maxHealth << "\n";
    std::cout << "Damage: " << damage << "\n";
    std::cout << "Position: (" << x << ", " << y << ")\n";
    std::cout << "Status: " << (alive ? "Alive" : "Dead") << "\n";
    std::cout << "====================\n";
}