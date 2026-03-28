#pragma once
#include "GameField.h"
#include "Enemy.h"
#include "../game_spells/DirectDamageSpell.h"
#include <vector>
#include <string>

class EnemyBuilding {
private:
    int health;
    int posX, posY;
    char symbol;
    int spawnCounter;
    int spawnInterval;

public:
    // Конструктор
    EnemyBuilding(int health, int startX, int startY, int spawnInterval, char symbol = 'X');

    // Спавн врагов
    void update(GameState& state, const GameField& field, int level); 

    void takeDamage(int damage);

    // Геттеры
    bool isAlive() const;
    int getHealth() const;
    int getX() const;
    int getY() const;
    char getSymbol() const;
};