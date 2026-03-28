#include "DirectAttackSpell.h"

static bool watch_radius(int sx, int sy, int tx, int ty, int r) {
    return (std::abs(sx - tx) + std::abs(sy - ty)) <= r;
}

bool DirectAttack::cast(Player& player, Gamefield& field, int target_x, int target_y) {
    Cell* cell = field.get_cell(target_x, target_y);
    if (!cell) { return false; }
    if (!watch_radius(player.get_x(), player.get_y(), target_x, target_y, r)) {
        std::cout << "Target is far\n";
        return false;
    }

    if (cell->is_occupied() && cell != nullptr){
        Enemy* enemy = cell->get_enemy();
        std::cout << "User casts " << n << " for " << dmg << " damage.\n";
        enemy->take_damage(dmg);
        if (enemy->hp <= 0) {
            std::cout << "Enemy defeated! (+10 points!)\n\n";
            player.edit_score(10);
            cell->set_enemy(nullptr);
        }
        return true;
    } else {
        return false;
    }
};

bool DirectAttack::tower_attack(Entity& attacker, Gamefield& grid, Player& player) {
    const int tx = player.get_x();
    const int ty = player.get_y();

    if (!watch_radius(attacker.x_pos, attacker.y_pos, tx, ty, r)) {
        return false;
    }

    std::cout << "Tower fires " << n << " for " << dmg << " damage.\n";
    player.take_damage(this->dmg);
    return true;
}

void DirectAttack::upgrade() {
    dmg += 10;
}
int DirectAttack::get_power() const {
    return dmg;
}