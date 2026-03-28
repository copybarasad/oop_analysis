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