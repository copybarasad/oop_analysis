#ifndef FIELD_H
#define FIELD_H

#include "randomiser.h"
#include "cell.h"
#include <vector>
#include <array>
#include <algorithm>

class Field{
    public:
    unsigned int height; // |
    unsigned int width;  // __
    std::vector<std::vector<Cell>> cells;

    std::array<unsigned int, 2> hero_coord; // x, y
    std::vector<std::array<unsigned int, 2>> character_coord; // x, y

    Field(unsigned int h = 10, unsigned int w = 10, unsigned int game_lvl = 1, unsigned int hard_lvl = 1);

    Field(const Field& field);

    Field& operator=(const Field& field);

    Field(Field&& field) noexcept;

    Field& operator=(Field&& field) noexcept;

    ~Field();

    bool damage_cell(unsigned int x, unsigned int y, unsigned int damage);

    void spawn_character(unsigned int x, unsigned int y, Character* character);

    void make_enemy(unsigned int x, unsigned int y, unsigned int game_lvl, unsigned int hard_lvl);

    void call_ally(unsigned int x, unsigned int y, unsigned int game_lvl, unsigned int hard_lvl);

    void set_trap(unsigned int x, unsigned int y, unsigned int damage);

    json to_json() const;
    
    void from_json(const json& j, unsigned int game_lvl, unsigned int hard_lvl);
};

#endif