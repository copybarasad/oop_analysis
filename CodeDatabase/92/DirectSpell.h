#pragma once
#include "Spell.h"

class DirectSpell : public Spell {
    public:
    DirectSpell(int d, int r, Type t) : Spell(d,r,t) {}
    std::string get_type_str() override { return "Прямой урон"; }
    bool use(Game& game, std::pair<int, int> caster, std::pair<int, int> target) override;
};
