#pragma once
#include "Spell.h"

class TrapSpell : public Spell {
public:
    TrapSpell(int d, int r, Type t) : Spell(d,r,t) {} ;
    std::string get_type_str() override { return "Ловушка"; }
    bool use(Game& game, std::pair<int, int> caster, std::pair<int, int> target) override;
};
