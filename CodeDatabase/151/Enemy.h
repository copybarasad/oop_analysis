#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Position.h"

class Enemy {
private:
    int health;
    int damage;
    Position position;

public:
    Enemy(int startX, int startY, int health = 30, int damage = 5);

    // Геттеры
    int getHealth() const;
    int getDamage() const;
    Position getPosition() const;

    // Сеттеры
    void setPosition(const Position& newPos);

    // Действия
    void takeDamage(int damage);
    bool isAlive() const;

    // ИИ для перемещения
    Position getNextMove(const Position& playerPos) const;
};

#endif