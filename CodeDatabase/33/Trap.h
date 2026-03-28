#pragma once
#include "Coord.h"

class Trap {
public:
    Trap(Coord pos, int damage);

    Coord Pos() const;
    int Damage() const;

private:
    Coord pos_;
    int damage_;
};
