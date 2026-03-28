#include "area_spell.h"
#include "field.h"
#include <cmath>

void AreaDamageSpell::use(Field& field, int caster_x, int caster_y, int target_x, int target_y) {
    int dist = abs(caster_x - target_x) + abs(caster_y - target_y);
    if (dist > radius) return;

    for (int dx = 0; dx < 2; ++dx) {
        for (int dy = 0; dy < 2; ++dy) {
            int tx = target_x + dx;
            int ty = target_y + dy;

            if (!field.is_valid_position(tx, ty)) continue;

            Enemy* enemy = field.get_enemy_at(tx, ty);
            if (enemy && enemy->is_alive()) {
                enemy->take_damage(damage);
            }

            for (auto& building : field.get_buildings()) {
                if (building->get_x() == tx && building->get_y() == ty && !building->is_destroyed()) {
                    building->take_damage(damage);
                }
            }
        }
    }
}

int AreaDamageSpell::get_radius() const {
    return radius; 
}

void AreaDamageSpell::set_radius(int r) {
    radius = r;
}