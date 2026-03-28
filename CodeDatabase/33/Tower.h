#pragma once
#include "Coord.h"

class Tower {
public:
    Tower(Coord pos, int spawn_interval);

    Coord Pos() const;
    int Interval() const;

    bool TickAndReady();

    void SetPos(Coord p);
    void SetInterval(int v);

    int  TicksLeft() const;
    void SetTicksLeft(int v);

private:
    Coord pos_;
    int interval_;
    int ticks_left_;
};
