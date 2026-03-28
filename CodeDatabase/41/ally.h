#ifndef ALLY_H
#define ALLY_H

#include "player.h"
#include <string>

class Game_field;

class Ally : public Player{
private:
    bool is_ally;
    int x;
    int y;

public:
    Ally(const std::string& ally_name, Game_field* game_field);
    void update();
    void move_towards_enemy(int enemy_x, int enemy_y);
    std::pair<int, int> get_position() const { return {x, y}; }
    void set_position(int new_x, int new_y) { x = new_x; y = new_y; }
};

#endif