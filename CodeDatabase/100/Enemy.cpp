#include "Enemy.h"
#include "GameField.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

// Конструктор
Enemy::Enemy(int health, int damage, int startX, int startY, char symbol) 
    : Character(health, damage, startX, startY, symbol) { }

// Передвижение
void Enemy::move(int playerX, int playerY, const GameField& field) {
    if (isAlive()) { 
        prevX = posX;
        prevY = posY;

        int deltaX = playerX - posX; // расстоние до игрока по оси X
        int deltaY = playerY - posY; // расстояние до игрока по оси Y

        // Если по оси X расстояние ненулевое, пытаемся двигаться по ней
        if (deltaX != 0) {
            int stepX = (deltaX > 0) ? 1 : -1;
            int newX = posX + stepX;
            if (field.isCellPassable(newX, posY)) {
                posX = newX;
                return;
            }
        }

        // Если по X не получилось, пробуем по Y
        if (deltaY != 0) {
            int stepY = (deltaY > 0) ? 1 : -1;
            int newY = posY + stepY;
            if (field.isCellPassable(posX, newY)) {
                posY = newY;
                return;
            }
        }   
    } 
}