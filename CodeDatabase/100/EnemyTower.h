#pragma once
#include "GameField.h"
#include "../game_spells/DirectDamageSpell.h"

class EnemyTower {
private:
    int health;
    int posX, posY;
    char symbol;
    int attackCooldown;
    int attackDelay;
    DirectDamageSpell* attackSpell;

public:
    EnemyTower(int health, int startX, int startY, char symbol='T');
    
    // Атаковать игрока
    void attackPlayerIfRange(Player& player, GameState& state);

    // Боёвка
    void takeDamage(int damage);

    // Геттеры
    bool isAlive() const;
    int getHealth() const;
    int getX() const;
    int getY() const;
    char getSymbol() const;

};