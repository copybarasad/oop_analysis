#include "Fireball.h"
#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include <iostream>
#include <cmath>

Fireball::Fireball(int damage, int radius)
    : Damage(damage), Radius(radius) {}

void Fireball::cast(Player& player, std::vector<Enemy>& enemies, Field& field) {
    std::cout << "Вы используете огненный шар!\n";
    
    int playerX = player.getX();
    int playerY = player.getY();
    
    std::cout << "Доступные цели в радиусе " << Radius << ":\n";
    bool foundTarget = false;
    
    for (size_t i = 0; i < enemies.size(); i++) {
        if (!enemies[i].isAlive()) continue;
        
        int enemyX = enemies[i].getX();
        int enemyY = enemies[i].getY();
        int distance = std::abs(enemyX - playerX) + std::abs(enemyY - playerY);
        
        if (distance <= Radius) {
            std::cout << i + 1 << ". Враг на (" << enemyX << ", " << enemyY << ") - расстояние: " << distance << "\n";
            foundTarget = true;
        }
    }
    
    if (!foundTarget) {
        std::cout << "Нет врагов в радиусе!\n";
        return;
    }
    
    int targetIndex;
    std::cout << "Выберите цель: ";
    std::cin >> targetIndex;
    
    if (targetIndex < 1 || targetIndex > (int)enemies.size()) {
        std::cout << "Неверный номер цели!\n";
        return;
    }
    
    Enemy& target = enemies[targetIndex - 1];
    if (!target.isAlive()) {
        std::cout << "Цель уже мертва!\n";
        return;
    }
    
    int distance = std::abs(target.getX() - playerX) + std::abs(target.getY() - playerY);
    if (distance > Radius) {
        std::cout << "Цель вне радиуса заклинания!\n";
        return;
    }
    
    target.takeDamage(Damage);
    std::cout << "Огненный шар попал во врага! Нанесено " << Damage << " урона.\n";
}

std::string Fireball::getName() const {
    return "Огненный шар";
}