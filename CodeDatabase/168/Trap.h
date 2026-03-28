#pragma once

#include "Position.h"

class Trap {
private:
    Position position;
    int damage;

public:
    Trap(int x, int y, int damage = 20);

    int get_Damage() const;
    const Position& get_Position() const;
};







