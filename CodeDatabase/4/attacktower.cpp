#include "attacktower.hpp"
#include "field.hpp"
#include "entitymanager.hpp"
#include "hero.hpp"
#include <cmath>

AttackTower::AttackTower(int id)
    : Entity(id, 200, 0), fireball_spell(30, 3) 
{
}

bool AttackTower::in_range(int hx, int hy, int tx, int ty) const {
    int dx = std::abs(hx - tx);
    int dy = std::abs(hy - ty);
    return dx <= fireball_spell.radius && dy <= fireball_spell.radius && (dx > 0 || dy > 0);
}

void AttackTower::try_attack(Field& field, EntityManager& roster, Hero& player, int tower_x, int tower_y) {
    if (current_cooldown > 0 || !alive()) return;
    if (in_range(field.x, field.y, tower_x, tower_y)) {
        bool success = fireball_spell.cast(field, roster, player, field.x, field.y);
        if (success) {
            current_cooldown = cooldown;
        }
    }
}

void AttackTower::tick() {
    if (current_cooldown > 0) {
        --current_cooldown;
    }
}