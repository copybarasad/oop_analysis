#include "direct_attack.h"
#include "grid.h"
#include "player.h"
#include "unit.h"
#include "point.h"
#include <cmath>

bool DirectAttack::in_radius_manhattan(int sx,int sy,int tx,int ty) const {
    return std::abs(sx - tx) + std::abs(sy - ty) <= r;
}

bool DirectAttack::cast(Player& player, Grid& grid, int target_x, int target_y) {
    Point* p = grid.getPoint(target_x, target_y);
    if (!p) { return false; }
    if (!in_radius_manhattan(player.get_x(), player.get_y(), target_x, target_y)) {
    }
    if (!p->is_occupied() || p->get_occupier() == nullptr) {
        return false;
    }
    Unit* occ = p->get_occupier();
    if (occ->is_friendly()) {
        return false;
    }
    occ->take_damage(dmg);
    if (occ->get_hp() <= 0) {
        player.add_score(10);
        p->set_occupier(nullptr);
    }
    return true;
}

bool DirectAttack::tower_attack(Unit& attacker, Grid& grid, Player& player) {
    (void)grid;
    const int tx = player.get_x();
    const int ty = player.get_y();

    if (!in_radius_manhattan(attacker.get_x(), attacker.get_y(), tx, ty)) {
        return false;
    }
    player.take_damage(dmg);
    return true;
}