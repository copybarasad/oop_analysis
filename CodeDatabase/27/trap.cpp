#include "trap.h"
#include "grid.h"
#include "player.h"
#include "point.h"
#include <cmath>

bool Trap::in_radius_manhattan(int sx,int sy,int tx,int ty) const {
    return std::abs(sx - tx) + std::abs(sy - ty) <= r;
}

bool Trap::cast(Player& player, Grid& grid, int target_x, int target_y) {
    Point* p = grid.getPoint(target_x, target_y);
    if (!p) {
        return false;
    }
    if (!in_radius_manhattan(player.get_x(), player.get_y(), target_x, target_y)) {
        return false;
    }
    if (!p->is_passable() || p->is_occupied()) {
        return false;
    }
    p->place_trap(dmg);
    return true;
}
