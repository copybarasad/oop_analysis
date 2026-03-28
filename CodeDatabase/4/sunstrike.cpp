#include "sunstrike.hpp"
#include "field.hpp"
#include "entitymanager.hpp"
#include "hero.hpp"
#include <iostream>
#include <cmath>

Sunstrike::Sunstrike(int dmg, int a) : base_damage(dmg), area(a) {}

void Sunstrike::upgrade() {
    area++;
    is_upgraded = true;
    upgrade_count++;
}

bool Sunstrike::cast(Field& field, EntityManager& roster, Hero& player, int cx, int cy) {
    int hx = field.x, hy = field.y;
    if (std::abs(cx - hx) > 5 || std::abs(cy - hy) > 5) {
        std::cout << "Cast point too far!\n";
        return false;
    }
    int half = area / 2;
    bool hit = false;
    for (int dy = -half; dy <= half; ++dy) {
        for (int dx = -half; dx <= half; ++dx) {
            int x = cx + dx, y = cy + dy;
            if (field.isValidPosition(x, y)) {
                auto ent = field.box[y][x].get_entity();
                if (ent == MOB || ent == TOWER || ent == ATTACK_TOWER) {
                    int id = field.box[y][x].id;
                    roster.atack_enemy(base_damage, id);
                    hit = true;
                }
            }
        }
    }
    return true;
}

std::string Sunstrike::name() const {
    return is_upgraded ? "Sunstrike+" : "Sunstrike";
}

int Sunstrike::cost() const {
    return 30;
}

void Sunstrike::load_state(std::istream& in) {
    int saved_upgrades;
    in.read(reinterpret_cast<char*>(&saved_upgrades), sizeof(saved_upgrades));
    upgrade_count = saved_upgrades;
    is_upgraded = (saved_upgrades > 0);
    area = 2 + saved_upgrades;
}