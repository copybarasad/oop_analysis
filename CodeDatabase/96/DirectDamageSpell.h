#pragma once
#include <vector>
#include <utility>
#include "Spell.h"
#include <memory>
#include "SpellContext.h"


class DirectDamageSpell : public Spell {
private:
    int damage;
    int range;
public:
    DirectDamageSpell(int dmg, int rng) : damage(dmg), range(rng) {}
    DirectDamageSpell() : damage(50), range(5) {}
    bool Cast(SpellContext& ctx) override;
    void enhance() override;

    std::shared_ptr<dataSpell> Save() override;
};