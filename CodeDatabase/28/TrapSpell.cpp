#include "TrapSpell.h"
#include "Gamefield.h"
#include "Player.h"
#include "Cell.h"
#include <iostream>
#include <cmath>

static bool within_radius(int sx, int sy, int tx, int ty, int r) {
    return (std::abs(sx - tx) + std::abs(sy - ty)) <= r;
}

bool Trap::cast(Player& player, Gamefield& grid, int target_x, int target_y) {
    Cell* cell = grid.get_cell(target_x, target_y);
    if (!cell) {
        std::cout << "Target out of bounds.\n";
        return false;
    }
    if (!within_radius(player.get_x(), player.get_y(), target_x, target_y, r)) {
        std::cout << "Target outside hack radius.\n";
        return false;
    }
    if (!cell->is_passable() || cell->is_occupied()) {
        std::cout << "Cannot place trap on blocked/occupied cell.\n";
        return false;
    }
    std::cout << "Player deploys a trap with damage = " << dmg << ".\n";
    cell->place_trap(dmg);
    return true;
}

void Trap::upgrade() {
    dmg += 10;
    r += 1;
}
int Trap::get_power() const {
    return dmg;
}