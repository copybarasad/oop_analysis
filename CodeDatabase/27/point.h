#pragma once
#include <iostream>

class Unit;
class Grid;

class Point {
private:
    int x_pos;
    int y_pos;
    bool passable = true;
    Unit* occupier = nullptr;
    Grid* parent_grid = nullptr;
    int trap_damage = 0;
    
public:
    void set_occupier(Unit* unit);
    Unit* get_occupier() const;

    void set_passable(bool is);
    bool is_passable() const;

    void set_x(int x);
    void set_y(int y);
    int x_position() const;
    int y_position() const;
    
    bool is_occupied() const;
    bool is_occupied_by_player() const;
    bool is_occupied_by_enemy() const;

    void set_parent_grid(Grid* grid);
    Grid* get_parent_grid() const;

    bool has_trap() const { return trap_damage > 0; }
    void place_trap(int damage);
    int get_trap_damage() const { return trap_damage; }
    int pop_trap_damage();
    void remove_occupier() { occupier = nullptr; }
};