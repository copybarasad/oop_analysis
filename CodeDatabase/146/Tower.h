#pragma once
#include "Building.h"

class GameArea;


class Tower : public Building {
private:
    int step = 3;
    int step_cd = 3;
    int radius = 2;
    double damage = 10;
public:
    Tower(double h, Coords crd) : Building(h, crd) {}
    void do_turn(GameArea& gamearea);
};
