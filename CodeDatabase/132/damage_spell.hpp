#pragma once

#include "spell.hpp"
#include "effect.hpp"
#include "IEntity.hpp"
#include <set>

const int RANGE = 10;

class DamageSpell : public Spell {
protected:
    int range;
    int area_r;
    std::set<Effect> effects;
public:
    DamageSpell(int range, int area_r, std::set<Effect> effects, char symbol);
    
    virtual int apply_enhancement(int current_lvl) = 0;
    virtual bool use(FieldHandler& field_handler, const Position& from, const Position& target) = 0;
};