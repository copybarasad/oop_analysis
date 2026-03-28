#include "Lighting.h"
#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include <iostream>
#include <cmath>

Lighting::Lighting(int damage, int radius)
    : Damage(damage), Radius(radius) {}

void Lighting::cast(Player& player, std::vector<Enemy>& enemies, Field& field) {
    std::cout << "Вы используете молнию!\n";
    std::cout << "Введите координаты левого верхнего угла области (" << Radius << "x" << Radius << "): ";
    
    int targetX, targetY;
    std::cin >> targetX >> targetY;
    
    if (targetX < 0 || targetY < 0 || 
        targetX + Radius > field.getWidth() || 
        targetY + Radius > field.getHeight()) {
        std::cout << "Область выходит за границы поля!\n";
        return;
    }
    
    int enemiesHit = 0;
    for (int x = targetX; x < targetX + Radius; x++) {
        for (int y = targetY; y < targetY + Radius; y++) {
            for (auto& enemy : enemies) {
                if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) {
                    enemy.takeDamage(Damage);
                    enemiesHit++;
                    std::cout << "Молния поразила врага на (" << x << ", " << y << ")!\n";
                }
            }
        }
    }
    
    if (enemiesHit == 0) {
        std::cout << "Молния ударила в пустоту!\n";
    } else {
        std::cout << "Поражено врагов: " << enemiesHit << "\n";
    }
}

std::string Lighting::getName() const {
    return "Молния";
}