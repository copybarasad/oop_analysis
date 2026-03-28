#pragma once
#include <vector>
#include <utility>
#include "Spell.h"
#include <memory>


class AreaDamageSpell : public Spell {
private:
    int damage;
    int radius;
public:
    AreaDamageSpell(int dmg, int rad) : damage(dmg), radius(rad) {}
    AreaDamageSpell() : damage(30), radius(2) {}
    bool Cast(SpellContext& ctx) override;
    void enhance() override;
    std::shared_ptr<dataSpell> Save() override;
};