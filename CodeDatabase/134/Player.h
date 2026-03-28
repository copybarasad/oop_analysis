#pragma once

#include "Hand.h"
#include "Spell.h"
#include "GameField.h"

#include <string>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>


class Player {
public:
    Player(int maxHealth, int damage); 

    void Move(int dx, int dy);
    void TakeDamage(int damage_amount);
    bool IsAlive() const;

    int GetX() const;
    int GetY() const;
    std::pair<int, int> GetPos() const;
    int GetHealth() const;
    int GetDamage() const;
    void UseSpell(int index, GameField* field, int targetX, int targetY);
    void Attack(GameField& field);


private:
    int health;
    int maxHealth;
    int attackDamage;
    int posX;
    int posY;
    Hand hand;
};
