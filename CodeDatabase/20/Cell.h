#pragma once
#include <iostream>
#include "Entity.h"
#include "Constants.h"

class Player;
class Enemy;
class Entity;

class Cell {
private:
    bool occupied = false;
    bool passable = true;

    Entity* occupied_by = nullptr;

    int x_pos;
    int y_pos;

    bool traped;

public:
    void set_occupied(bool is);
    void set_passable(bool is);
    void set_occupied_by(Entity* et);
    void set_traped(bool is);


    void set_x(int x);
    void set_y(int y);

    bool is_occupied();
    Entity* is_occupied_by();
    bool is_passable();
    bool is_traped();



    int x_position();
    int y_position();

};
