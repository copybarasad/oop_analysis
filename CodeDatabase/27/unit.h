#pragma once
#include "point.h"
#include "grid.h"

class Unit {
protected:
    Grid* grid;
    static constexpr int DEFAULT_HP = 100;
    static constexpr int DEFAULT_DAMAGE = 10;
    static constexpr int DEFAULT_X = 0;
    static constexpr int DEFAULT_Y = 0;

    int hp;
    int damage;
    bool friendly;
    int x_pos;
    int y_pos;
    Point* current_point = nullptr;

    virtual void on_move_to_point(Point& point) = 0;

private:
    bool is_adjacent_position(Point* target_point) const;

public:
    Unit(Grid* grid, int hp, int damage, bool friendly, int x, int y);
    virtual ~Unit() = default;

    void go_to(Point* target_point);
    
    void take_damage(int amount) {
        hp -= amount;
        if (hp < 0) hp = 0;
    }
    
    int get_hp() const { return hp; }
    int get_damage() const { return damage; }
    bool is_friendly() const { return friendly; }
    int get_x() const { return x_pos; }
    int get_y() const { return y_pos; }
    Point* get_current_point() const { return current_point; }
    void clear_current_point() { current_point = nullptr; }
    void set_parent_grid(Grid* new_grid) { grid = new_grid; }
    Grid* get_parent_grid() const { return grid; }
    void set_hp(int v) { hp = v; if (hp < 0) hp = 0; }
    void set_damage(int v) { damage = v; }
};