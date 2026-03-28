#include "Tower.h"

Tower::Tower(Coord pos, int spawn_interval)
    : pos_(pos), interval_(spawn_interval), ticks_left_(spawn_interval) {}

Coord Tower::Pos() const { return pos_; }
int Tower::Interval() const { return interval_; }

void Tower::SetPos(Coord p) { pos_ = p; }

void Tower::SetInterval(int v) { interval_ = v; }

int Tower::TicksLeft() const { return ticks_left_; }

void Tower::SetTicksLeft(int v) { ticks_left_ = v; }

bool Tower::TickAndReady() {
    if (ticks_left_ > 0) --ticks_left_;
    if (ticks_left_ == 0) { ticks_left_ = interval_; return true; }
    return false;
}
