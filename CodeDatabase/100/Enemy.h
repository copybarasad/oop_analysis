#pragma once
#include "Character.h"

class Enemy : public Character {
public:
    // Конструктор
    Enemy(int health, int damage, int startX, int startY, char symbol = 'E');

    // Передвижение
    void move(int playerX, int playerY, const GameField& field);
};
