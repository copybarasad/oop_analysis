#include "weapon.hpp"

Weapon::Weapon(CombatMode combat_type, std::set<Effect> bullet_effects):
    combat_type(combat_type), bullet_effects(bullet_effects) {};

std::set<Effect> Weapon::use() {
    return bullet_effects;
}

CombatMode Weapon::combat_mode() {
    return combat_type;
}