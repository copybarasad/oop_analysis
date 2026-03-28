#pragma once

#include "Position.h"

class Enemy {
private:
    int health;
    int damage;
    Position position;

public:
    Enemy(int start_x, int start_y, int health = 30, int damage = 10);

    void take_Damage(int damage);
    bool isAlive() const;
    int get_Health() const;
    int get_Damage() const;
    const Position& get_Position() const;

    void set_Position(const Position& newPosition);
    
    void setHealth(int h);
};