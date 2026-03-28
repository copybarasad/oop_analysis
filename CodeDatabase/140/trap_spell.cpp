#include "trap_spell.h"
#include "field.h"
#include <cmath>

void TrapSpell::use(Field& field, int caster_x, int caster_y, int target_x, int target_y) {
    int dist = abs(caster_x - target_x) + abs(caster_y - target_y);
    if (dist > radius) return;

    if (!field.is_valid_position(target_x, target_y)) return;
    if (field.get_cell_type(target_x, target_y) != CellType::EMPTY) return;

    field.place_trap(target_x, target_y, damage);
}

int TrapSpell::get_damage() const {
    return damage;
}

void TrapSpell::set_damage(int d) {
    damage = d;
}