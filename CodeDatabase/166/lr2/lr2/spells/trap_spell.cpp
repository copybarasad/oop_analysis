#include "spells/trap_spell.hpp"
#include "field.hpp"
#include "hero.hpp"
#include "enemy.hpp"
#include "spawner.hpp"

extern void trap_register_created(const Position& pos, int dmg);

bool TrapSpell::cast(Hero& hero,
                     Field& field,
                     std::vector<Enemy>&,
                     std::vector<Spawner>&,
                     BoostState& boosts,
                     const Position& target)
{
    (void)field;
    int dx = std::abs(hero.getPosition().getX() - target.getX());
    int dy = std::abs(hero.getPosition().getY() - target.getY());
    if (dx+dy != 1) return false;

    int realDmg = dmg_ + boosts.trapDamagePlus;
    boosts.clear();

    trap_register_created(target, realDmg);
    return true;
}
