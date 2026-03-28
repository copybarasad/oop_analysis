#pragma once

#include "Cell_type.h"
#include "Position.h"

class Cell {
private:
    CellType type;
    bool Player_on;
    bool Enemy_on;
    bool Building_on;
    bool Tower_on;
    bool Ally_on;
    int trapDamage;

public:
    Cell(CellType type = CellType::EMPTY);

    bool is_Passable() const;
    bool is_Empty() const;
    CellType get_type() const;

    void set_Player(bool is_set);
    void set_Enemy(bool is_set);
    void set_Building(bool is_set);
    void set_Tower(bool is_set);
    void set_Ally(bool is_set);
    void set_Trap(int damage);
    void clear_Trap();

    bool has_Player() const;
    bool has_Enemy() const;
    bool has_Building() const;
    bool has_Tower() const;
    bool has_Ally() const;
    bool has_Trap() const;
    int get_TrapDamage() const;

    void clear();
};
