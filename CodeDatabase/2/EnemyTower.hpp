#pragma once

#include "Board.hpp"

class EnemyTower {
private:
    int countdown;
    int x, y;
    int hp;
    int level;
public:
    EnemyTower(int x_pos, int y_pos, int start_level);
    bool spawn_enemy(int& spawn_x, int& spawn_y, Board& board);
    void take_damage(const int player_damage) ;
    bool is_death() const;
    void get_coords(int& cur_x, int& cur_y);
    void setLevel(int new_level);
    int get_hp(){return hp;}
    void set_hp(int new_hp) { hp = new_hp; }
    void set_position(int new_x,int new_y) ;
};