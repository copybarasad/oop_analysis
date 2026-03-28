#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "field/point.hpp"

struct Cursor {
    Point position {0, 0};
    bool is_enabled {false};
    int area_size {1};
    int radius {0};
};

#endif

#ifndef DRAW_HPP
#define DRAW_HPP

#include "entities/player.hpp"
#include "field/field.hpp"
#include "cursor.hpp"
#include "spells/hand.hpp"

class Draw {
    Field &field;
    Cursor &cursor;
    Hand &hand;
    Player &player;

public:
    Draw(Field &field, Cursor &cursor, Hand &hand, Player &player) : field(field), cursor(cursor), hand(hand), player(player) {}
    void draw_field();
    void draw_score();
    void print_pricelist();
    void print_info();
};

#endif