#pragma once

#include "Position.h"

class Ally {
private:
    Position position;
    int health;
    int damage;

public:
    Ally(int x, int y, int health = 20, int damage = 15);

    void take_Damage(int dmg);
    bool isAlive() const;
    int get_Health() const;
    int get_Damage() const;
    const Position& get_Position() const;
    void set_Position(const Position& newPos);
};
