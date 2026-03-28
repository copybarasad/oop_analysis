#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <vector>
#include "game_cell.h"

class game_field {
    std::vector<std::vector<game_cell>> cells;
    int width;
    int height;
public:
    game_field(int input_width, int input_height);
    game_field(const game_field& other);
    game_field(game_field&& other) noexcept;
    ~game_field();

    int get_width();
    int get_height();
    game_cell& get_cell(int x, int y);
    bool isValidPosition(int x, int y) const;
    void set_player(int x, int y);
    void set_enemy(int x, int y);
    std::vector<std::vector<game_cell>> get_cells();
    void print_field();
};

#endif
