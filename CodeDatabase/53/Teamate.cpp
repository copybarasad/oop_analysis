#include "Teamate.h"
#include "GameField.h"
#include "Enemy.h"
#include "Player.h"
#include <cmath>
#include <iostream>
#include <limits>

using namespace GameConstants;

Teamate::Teamate(std::string name, int health, int damage, int x, int y, int attackRange)
    : Character(name, health, damage, x, y), attackRange(attackRange) {}

void Teamate::update(GameField& field, Player& player) {
    if (!isAlive()) return;
    
    int targetX, targetY;
    if (findNearestEnemy(field, targetX, targetY)) {
        int distance = std::abs(targetX - position.x) + std::abs(targetY - position.y);
        
        if (distance <= attackRange) {
            Enemy& enemy = field.findEnemy(targetX, targetY);
            attackEnemy(enemy, player);
        } else {
            moveTowardsEnemy(field);
        }
    }
}

void Teamate::moveTowardsEnemy(GameField& field) {
    int targetX, targetY;
    if (!findNearestEnemy(field, targetX, targetY)) return;
    
    int newX = position.x;
    int newY = position.y;
    

    if (position.x < targetX && field.canMoveTo(position.x + 1, position.y)) {
        newX = position.x + 1;
    } else if (position.x > targetX && field.canMoveTo(position.x - 1, position.y)) {
        newX = position.x - 1;
    }

    else if (position.y < targetY && field.canMoveTo(position.x, position.y + 1)) {
        newY = position.y + 1;
    } else if (position.y > targetY && field.canMoveTo(position.x, position.y - 1)) {
        newY = position.y - 1;
    }
    
    Type_cell cell = field.getCell(newX, newY).getType();
    if ((newX != position.x || newY != position.y) && field.canMoveTo(newX, newY) && 
        cell != PLAYER && cell != TRAP) {
            field.setCellType(position.x, position.y, EMPTY);
            position.x = newX;
            position.y = newY;
            field.setCellType(newX, newY, JHONNYGHOST);
    }
}

void Teamate::attackEnemy(Enemy& enemy, Player& player) {
    std::cout << "Союзник " << name << " атакует " << enemy.getName() << " и наносит " << damage << " урона!" << std::endl;
    enemy.take_damage(damage);
    
    if (!enemy.isAlive()) {
        std::cout << "Союзник " << name << " побеждает " << enemy.getName() << "!" << std::endl;
        player.takeAwards(ENEMY);
    }
}

bool Teamate::findNearestEnemy(GameField& field, int& targetX, int& targetY) {
    int minDistance = std::numeric_limits<int>::max();
    bool found = false;
    
    for (const auto& enemy : field.arr_enemies) {
        if (enemy && enemy->isAlive()) {
            int distance = std::abs(enemy->getX() - position.x) + std::abs(enemy->getY() - position.y);
            if (distance < minDistance) {
                minDistance = distance;
                targetX = enemy->getX();
                targetY = enemy->getY();
                found = true;
            }
        }
    }
    
    return found;
}