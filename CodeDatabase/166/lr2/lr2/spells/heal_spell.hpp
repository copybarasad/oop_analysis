#ifndef HEAL_SPELL_HPP_INCLUDED
#define HEAL_SPELL_HPP_INCLUDED

#include "spell.hpp"
#include <string>

// простое вспомогательное заклинание: лечит героя на фиксированное значение
class HealSpell : public Spell {
public:
    explicit HealSpell(int amount) : amount_(amount) {}

    std::string name() const override {
        return "heal +" + std::to_string(amount_);
    }

    bool apply(Hero& hero, Field& /*field*/,
               std::vector<Enemy>& /*enemies*/,
               std::vector<Spawner>& /*spawners*/) override;

private:
    int amount_{1};
};

#endif // HEAL_SPELL_HPP_INCLUDED
