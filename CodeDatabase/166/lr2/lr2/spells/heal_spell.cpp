#include "spells/heal_spell.hpp"
#include "hero.hpp"

// просто лечим героя и считаем, что действие всегда успешно
bool HealSpell::apply(Hero& hero, Field&, std::vector<Enemy>&, std::vector<Spawner>&) {
    hero.heal(amount_);
    return true;
}
