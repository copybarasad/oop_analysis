#include "create_spell.h"
#include "field.h"
#include "ally.h"
#include <iostream>

CreateSpell::CreateSpell(int hp, int dmg, int rad)
    : ally_hp(hp), ally_damage(dmg), radius(rad) {}

void CreateSpell::use(Field& field, int caster_x, int caster_y, int target_x, int target_y) {
    int dist = abs(caster_x - target_x) + abs(caster_y - target_y);
    if (dist > radius) {
        cout << "Create failed: too far!\n";
        return;
    }

    if (!field.is_valid_position(target_x, target_y) || field.is_cell_occupied(target_x, target_y)) {
        cout << "Create failed: invalid or occupied cell!\n";
        return;
    }

    for (int i = 0; i < ally_count; ++i) {
        int ax = target_x, ay = target_y;

        if (i > 0) {
            bool found = false;
            for (int dy = -1; dy <= 1 && !found; ++dy) {
                for (int dx = -1; dx <= 1 && !found; ++dx) {
                    int nx = target_x + dx;
                    int ny = target_y + dy;
                    if (field.is_valid_position(nx, ny) && !field.is_cell_occupied(nx, ny)) {
                        ax = nx;
                        ay = ny;
                        found = true;
                    }
                }
            }
        }
        field.create_ally(ax, ay, ally_hp, ally_damage);
    }
    cout << "Ally created at (" << target_x << "," << target_y << ")!\n";
}

void CreateSpell::set_ally_count(int count) {
    ally_count = count;
}

int CreateSpell::get_ally_count() const {
    return ally_count;
}