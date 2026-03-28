#pragma once
#include "Character.h"

class Ally : public Character {
public:
    // Конструктор
    Ally(int health, int damage, int startX, int startY, char symbol = 'A');

    // Передвижение
    void move(int deltaX, int deltaY, const GameField& field) override;
};