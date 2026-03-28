#ifndef MOVEMENT_H
#define MOVEMENT_H
#include "player.h"
#include "place.h"

class Movement{
private:
    Player& player;
    Place& place;
public:
    enum class Direction {Up, Left, Dawn, Right};

    Movement(Player& player_, Place& place_);

    int delta_x(Direction direct);

    int delta_y(Direction direct);

    int move(Direction direct = {Direction::Up});

    ~Movement();
};

#endif