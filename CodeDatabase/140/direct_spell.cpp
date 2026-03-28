#include "direct_spell.h"
#include "field.h"
#include <cmath>

void DirectDamageSpell::use(Field& field, int caster_x, int caster_y, int target_x, int target_y) {
    int dist = abs(caster_x - target_x) + abs(caster_y - target_y);
    if (dist > radius) return;

    Enemy* enemy = field.get_enemy_at(target_x, target_y);
    
    if (enemy && enemy->is_alive()) {
        enemy->take_damage(damage);
        return;
    }

    for (auto& building : field.get_buildings()) {
        if (building->get_x() == target_x && building->get_y() == target_y && !building->is_destroyed()) {
            building->take_damage(damage);
            return;
        }
    }
}

int DirectDamageSpell::get_radius() const {
    return radius;
}

void DirectDamageSpell::set_radius(int r) {
    radius = r;
}