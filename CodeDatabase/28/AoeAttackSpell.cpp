#include "AoeAttackSpell.h"

static bool watch_radius(int sx, int sy, int tx, int ty, int r) {
    return (std::abs(sx - tx) + std::abs(sy - ty)) <= r;
}

bool AOEAttack::cast(Player& player, Gamefield& field, int target_x, int target_y) {
    if (!watch_radius(player.get_x(), player.get_y(), target_x, target_y, r)) {
        std::cout << "Target is far\n";
        return false;
    }
    bool any_cell = false;
    std::cout << "User casts " << n << " dealing " << dmg << " in "<< r << "x" << r << "area.\n";
    for (int dy = 0; dy < r; ++dy) {
        for (int dx = 0; dx < r; ++dx) {
            Cell* cell = field.get_cell(target_x + dx, target_y + dy);
            if (!cell) continue;
            any_cell = true;
            if (cell->is_occupied() && cell != nullptr) {
                Enemy* enemy = cell->get_enemy();
                enemy->take_damage(dmg);
                if (enemy->get_hp() <= 0) {
                    std::cout << "Enemy defeated! (+10 points!)\n";
                    player.edit_score(10);
                    cell->set_enemy(nullptr);
                }
            }
        }
    }
    if (!any_cell) {
        std::cout << "Area outside grid. Hack still used.\n";
    }
    return true;
}

void AOEAttack::upgrade() {
    dmg += 15;
    r += 1;
}

int AOEAttack::get_power() const {
    return dmg;
}
