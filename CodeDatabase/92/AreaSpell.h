#pragma once
#include "Spell.h"
#include <vector>

class AreaSpell : public Spell {
     public:
     AreaSpell(int d, int r, Type t) : Spell(d,r,t) {}
     std::vector<std::pair<int, int>> get_targeted_cells(std::pair<int, int> target);
     std::string get_type_str() override { return "Урон по площади 2x2"; }
     bool use(Game& game, std::pair<int, int> caster, std::pair<int, int> target) override;
};
