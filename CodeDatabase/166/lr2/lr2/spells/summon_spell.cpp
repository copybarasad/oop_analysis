#include "spells/summon_spell.hpp"
#include "field.hpp"
#include "hero.hpp"
#include "enemy.hpp"
#include "spawner.hpp"

extern void summon_register_created(const std::vector<Position>& ps, int hp, int dmg);

bool SummonSpell::cast(Hero& hero,
                       Field& field,
                       std::vector<Enemy>& enemies,
                       std::vector<Spawner>& spawners,
                       BoostState& boosts,
                       const Position& target)
{
    (void)enemies;  // не используем
    (void)spawners; // не используем

    // цель должна быть строго соседней по ортогонали
    int dx = std::abs(hero.getPosition().getX() - target.getX());
    int dy = std::abs(hero.getPosition().getY() - target.getY());
    if (dx + dy != 1) return false;

    // итоговое количество с учётом буста
    int toSummon = baseCount_ + boosts.summonCountPlus;
    boosts.clear();

    std::vector<Position> placed;
    auto place_if_ok = [&](const Position& p) {
        if ((int)placed.size() >= toSummon) return;
        if (field.inBounds(p.getX(), p.getY()) && field.passable(p.getX(), p.getY())) placed.push_back(p);
    };

    place_if_ok(target);
    static const Position dirs[4]{{1,0},{-1,0},{0,1},{0,-1}};
    for (auto d : dirs) {
        if ((int)placed.size() >= toSummon) break;
        place_if_ok(Position{target.getX() + d.getX(), target.getY() + d.getY()});
    }

    if (placed.empty()) return false;

    summon_register_created(placed, allyHp_, allyDmg_);
    return true;
}