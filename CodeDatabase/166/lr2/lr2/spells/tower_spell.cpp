#include "spells/tower_spell.hpp"
#include "field.hpp"
#include "hero.hpp"
#include "enemy.hpp"
#include "spawner.hpp"

extern void tower_register_created(const Position& pos, int dmg, int rad);

bool TowerSpell::cast(Hero& hero,
                      Field& field,
                      std::vector<Enemy>&,
                      std::vector<Spawner>&,
                      BoostState& boosts,
                      const Position& target)
{
    int dx = std::abs(hero.getPosition().getX() - target.getX());
    int dy = std::abs(hero.getPosition().getY() - target.getY());
    if (dx+dy != 1) return false;
    if (!field.inBounds(target.getX(), target.getY())) return false;
    if (!field.passable(target.getX(), target.getY())) return false;

    tower_register_created(target, dmg_, radius_);
    boosts.clear();
    return true;
}
