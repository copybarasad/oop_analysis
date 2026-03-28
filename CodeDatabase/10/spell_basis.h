#pragma once
#include "spell.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "tower.h"
#include <cmath>
#include <iostream>

class SpellBasis : public Spell {
protected:
    std::string name;
    int cost;
    int range;
    int power;

public:
    SpellBasis(const std::string& spellName, int spellCost, int spellRange, int spellPower = 0)
        : name(spellName), cost(spellCost), range(spellRange), power(spellPower) {}

    virtual ~SpellBasis() = default;

    

    int getCost() const override { return cost; }
    int getRange() const override { return range; }
    std::string getName() const override { return name; }
    int getPower() const { return power; }

    virtual int cast_spell(int, int, int, int, Field&, Player&) override {
        return 0;
    }
    
    virtual void upgrade(int increment = constants::SPELL_DAMAGE_INCREMENT) {
        power += increment;
    }
};
