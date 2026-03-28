#pragma once
#include <vector>
#include <utility>
#include "Spell.h"
#include <memory>


class TrapSpell : public Spell {
private:
    int damage;
    int range;
public:
    TrapSpell(int dmg, int rng) : damage(dmg), range(rng) {}
    TrapSpell() : damage(20), range(3) {}
    bool Cast(SpellContext& ctx) override;
    void enhance() override;
    std::shared_ptr<dataSpell> Save() override;
};