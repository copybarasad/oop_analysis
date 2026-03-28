#pragma once
#include "Cell.h"

class Entity {
public:
    Cell* current_cell = nullptr;
    int hp;
    int x_pos;
    int y_pos;

    virtual ~Entity() = default;
    virtual void go_to(Cell& cell);
};
