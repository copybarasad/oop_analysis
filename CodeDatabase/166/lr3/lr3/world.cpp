#include "world.hpp"

#include <algorithm>
#include <utility>

World::World(Field f, Hero h, std::vector<Enemy> es, std::vector<Spawner> ss, std::size_t handCap)
    : field(std::move(f)),
      hero(std::move(h)),
      enemies(std::move(es)),
      spawners(std::move(ss)),
      hand(handCap) {}

int alive_enemies(const World& w) {
    return (int)std::count_if(w.enemies.begin(), w.enemies.end(), [](const Enemy& e){return !e.isDead();});
}

void maybe_award_card(World& w) {
    while (w.kills / 3 > w.awardsGiven) {
        if (!w.hand.drawRandom()) break;
        w.awardsGiven += 1;
    }
}

int max_enemy_hp_for_level(int level)   { return 3 + 2*std::max(1, level); }
int max_spawner_hp_for_level(int level) { return 10 + 2*std::max(1, level); }

bool is_enemy_at(const World& w, const Position& p, int* idx) {
    for (int i = 0; i < (int)w.enemies.size(); ++i) {
        if (!w.enemies[i].isDead() && w.enemies[i].getPosition() == p) {
            if (idx) *idx = i;
            return true;
        }
    }
    return false;
}

bool is_spawner_at(const World& w, const Position& p) {
    for (const auto& s : w.spawners) if (s.getPos() == p) return true;
    return false;
}

int trap_index_at(const World& w, const Position& p) {
    for (int i=0;i<(int)w.traps.size();++i) if (w.traps[i].getPos() == p) return i;
    return -1;
}

int tower_index_at(const World& w, const Position& p) {
    for (int i=0;i<(int)w.towers.size();++i) if (w.towers[i].getPos() == p) return i;
    return -1;
}

int spawner_index_at(const World& w, Position p) {
    for (int i = 0; i < (int)w.spawners.size(); ++i)
        if (w.spawners[i].getPos().getX() == p.getX() && w.spawners[i].getPos().getY() == p.getY())
            return i;
    return -1;
}

bool is_ally_at(const World& w, const Position& p) {
    for (const auto& a : w.allies) if (!a.isDead() && a.getPos() == p) return true;
    return false;
}

bool cell_free_common(const World& w, const Position& p, bool allowHero) {
    if (!w.field.inBounds(p.getX(),p.getY())) return false;
    if (!w.field.passable(p.getX(),p.getY())) return false;
    if (!allowHero && w.hero.getPosition() == p) return false;
    if (is_enemy_at(w,p)) return false;
    if (is_spawner_at(w,p)) return false;
    if (trap_index_at(w,p)  >= 0) return false;
    if (tower_index_at(w,p) >= 0) return false;
    if (is_ally_at(w,p)) return false;
    return true;
}

bool cell_free_for_enemy(const World& w, const Position& p) {
    return cell_free_common(w, p, false);
}

bool cell_free_for_hero(const World& w, const Position& p) {
    if (!w.field.inBounds(p.getX(),p.getY())) return false;
    if (!w.field.passable(p.getX(),p.getY())) return false;
    if (is_enemy_at(w,p)) return false;
    if (is_spawner_at(w,p)) return false;
    if (trap_index_at(w,p)  >= 0) return false;
    if (tower_index_at(w,p) >= 0) return false;
    if (is_ally_at(w,p)) return false;
    return true;
}

bool cell_free_for_ally(const World& w, const Position& p) {
    return cell_free_common(w, p, true);
}

