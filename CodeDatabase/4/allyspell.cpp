#include "allyspell.hpp"
#include "field.hpp"
#include "entitymanager.hpp"
#include "hero.hpp"
#include <iostream>
#include <vector>

AllySpell::AllySpell() = default;

void AllySpell::upgrade() {
    summon_count++;
    is_upgraded = true;
    upgrade_count++;
}

bool AllySpell::cast(Field& field, EntityManager& roster, Hero& player, int tx, int ty) {
    int hx = field.x, hy = field.y;
    std::vector<std::pair<int,int>> dirs = {{1,0},{-1,0},{0,1},{0,-1}};
    for (int c = 0; c < summon_count; ++c) {
        bool placed = false;
        for (auto [dx, dy] : dirs) {
            int nx = hx + dx, ny = hy + dy;
            if (field.canMoveTo(nx, ny)) {
                roster.create_ally(roster.ally_id_counter++, nx, ny);
                field.box[ny][nx] = FieldCell(EMPTY, ALLY, roster.ally_id_counter - 1);
                placed = true;
                break;
            }
        }
        if (!placed) break;
    }
    return true;
}

std::string AllySpell::name() const {
    return is_upgraded ? "Ally+" : "Ally";
}

int AllySpell::cost() const {
    return 20;
}

void AllySpell::load_state(std::istream& in) {
    int saved_upgrades;
    in.read(reinterpret_cast<char*>(&saved_upgrades), sizeof(saved_upgrades));
    upgrade_count = saved_upgrades;
    is_upgraded = (saved_upgrades > 0);
    summon_count = 1 + saved_upgrades;
}