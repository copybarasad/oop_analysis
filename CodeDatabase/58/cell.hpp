#pragma once

#include <iostream>
#include <random>

#include "entity.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemy_build.hpp"
#include "enemy_tower.hpp"
#include "enum_type_cell.hpp"

#define BASE_TYPE_CELL TypeCell::NORMAL

class Cell{
private:
    int x, y;
    TypeCell type_cell;
    Entity* entity{nullptr};

public:
    Cell(int x = 0, int y = 0, TypeCell type_cell = BASE_TYPE_CELL);
    ~Cell();

    int get_x();
    int get_y();
    void get_coord(int& x, int& y);

    Entity* get_entity() const;
    int set_entity(Entity* entity);
    int move_entity(Entity* entity, Cell& target_cell);

    void clear_entity();

    // void set_x(int x);
    // void set_y(int y);
    // void set_coord(int x, int y);

    TypeCell get_type_cell() const;
    void set_type_cell(TypeCell type_cell);
};