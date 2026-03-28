#include "spell_callbacks.hpp"

#include <tuple>
#include <utility>
#include <vector>

#include "rendering.hpp"
#include "world.hpp"

namespace {
    std::vector<Position> g_summon_positions;
    int g_summon_hp = 5, g_summon_dmg = 2;
    std::vector<std::pair<Position,int>> g_traps_created;
    std::vector<std::tuple<Position,int,int>> g_towers_created;
}

void summon_register_created(const std::vector<Position>& ps, int hp, int dmg) {
    g_summon_positions = ps;
    g_summon_hp = hp;
    g_summon_dmg = dmg;
}

void trap_register_created(const Position& pos, int dmg) {
    g_traps_created.emplace_back(pos, dmg);
}

void tower_register_created(const Position& pos, int dmg, int rad) {
    g_towers_created.emplace_back(pos, dmg, rad);
}

void flush_created_entities(World& w) {
    for (auto& p: g_summon_positions) { w.allies.push_back(Ally{p, g_summon_hp, g_summon_dmg}); draw_cell(w, p); }
    g_summon_positions.clear();

    for (auto& t : g_traps_created) { w.traps.push_back(Trap{t.first, t.second}); draw_cell(w, t.first); }
    g_traps_created.clear();

    for (auto& t : g_towers_created) {
        Position p; int dmg, rad;
        std::tie(p, dmg, rad) = t;
        w.towers.push_back(Tower(p, dmg, rad));
        draw_cell(w, p);
    }
    g_towers_created.clear();
}

