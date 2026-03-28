#ifndef CELL_H
#define CELL_H

#include "constants.h"

class Creature;

class Cell{
    int x;
    int y;
    Creature *placed_creature = nullptr;
    ObjectType object = ObjectType::Empty;
public:
    Cell() : x(0), y(0) {}
    Cell(int x, int y) : x(x), y(y) {}
    int getX() const { return x; }
    int getY() const { return y; }
    Creature *getCreature() const { return placed_creature; }
    char getObject() const { return static_cast<char>(object); }
    void place_object(ObjectType obj) { this->object = obj; }
    void place_object(ObjectType obj, Creature *creature) { this->object = obj; placed_creature = creature; }
    void remove_object() { this->object = ObjectType::Empty; placed_creature = nullptr; }
};

#endif