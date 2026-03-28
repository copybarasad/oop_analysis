#include "fireball.hpp"
#include "field.hpp"
#include "entitymanager.hpp"
#include "hero.hpp"
#include <iostream>
#include <cmath>

Fireball::Fireball(int dmg, int rad) 
    : base_damage(dmg), radius(rad), is_upgraded(false), upgrade_count(0) 
{}

void Fireball::upgrade() {
    radius += 3;
    is_upgraded = true;
    upgrade_count++;
}

bool Fireball::cast(Field& field, EntityManager& roster, Hero& player, int tx, int ty) {
    int hx = field.x, hy = field.y;
    if (std::abs(tx - hx) > radius || std::abs(ty - hy) > radius) {
        std::cout << "Target out of range!\n";
        return false;
    }
    if (field.isValidPosition(tx, ty)) {
        EntityType ent = field.box[ty][tx].get_entity();
        if (ent == MOB || ent == TOWER || ent == ATTACK_TOWER) {
            int id = field.box[ty][tx].id;
            roster.atack_enemy(base_damage, id);
            return true;
        }
    }
    std::cout << "No valid target at (" << tx << ", " << ty << ")\n";
    return false;
}

std::string Fireball::name() const {
    return is_upgraded ? "Fireball+" : "Fireball";
}

int Fireball::cost() const {
    return 10;
}

void Fireball::load_state(std::istream& in) {
    int saved_upgrades;
    in.read(reinterpret_cast<char*>(&saved_upgrades), sizeof(saved_upgrades));
    upgrade_count = saved_upgrades;
    is_upgraded = (saved_upgrades > 0);
    radius = 9 + (saved_upgrades * 3);
}