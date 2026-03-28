#pragma once

class Player;
class GameField;

class Spell {
public:
    virtual ~Spell() = default;
    virtual void Use(Player* player, GameField* field, int targetX, int targetY) = 0;
};


// добавить конструктор, где указвыается, какие данные нужно подавать на Use
