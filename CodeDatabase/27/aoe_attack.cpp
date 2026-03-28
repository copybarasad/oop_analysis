#include "aoe_attack.h"
#include "grid.h"
#include "player.h"
#include "unit.h"
#include "point.h"
#include <cmath>

bool AOEAttack::in_radius_manhattan(int sx,int sy,int tx,int ty) const {
    return std::abs(sx - tx) + std::abs(sy - ty) <= r;
}

bool AOEAttack::cast(Player& player, Grid& grid, int target_x, int target_y) {
    if (!in_radius_manhattan(player.get_x(), player.get_y(), target_x, target_y)) {
        return false;
    }    for (int dy = 0; dy < 2; ++dy) {
        for (int dx = 0; dx < 2; ++dx) {
            Point* p = grid.getPoint(target_x + dx, target_y + dy);
            if (!p) continue;
            if (p->is_occupied() && p->get_occupier() != nullptr) {
                Unit* occ = p->get_occupier();
                if (!occ->is_friendly()) {
                    occ->take_damage(dmg);
                    if (occ->get_hp() <= 0) {
                        player.add_score(10);
                        p->set_occupier(nullptr);
                    }
                }
            }
        }
    }
    return true;
}
