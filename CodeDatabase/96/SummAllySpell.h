#pragma once
#include "Spell.h"
#include <vector>
#include <utility>

class SummAllySpell : public Spell {
private:
    int numAllies;
public:
    SummAllySpell() : numAllies(1) {}
    SummAllySpell(int num) : numAllies(num) {}
    ~SummAllySpell() override = default;
    bool Cast(SpellContext& ctx) override;
    void enhance() override;
    std::shared_ptr<dataSpell> Save() override;
};