#pragma once
#include "Cell.hpp"

class TrapCell : public Cell {
private:
    int damage;
public:
    TrapCell(int damage) : Cell("./sprites/Trap.png", trap), damage(damage) {}

    int get_damage() { return damage; }
}; 