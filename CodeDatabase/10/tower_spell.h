#pragma once
#include "spell.h"
#include "field.h"
#include "constants.h"
#include "cell.h"
#include <string>

class TowerSpell : public Spell {
private:
    std::string name;
    int cost;
    int range;
    int power;

public:
    TowerSpell();
    
    bool canCast(int targetX, int targetY, int towerX, int towerY, Field& field) const;
    int cast_spell(int targetX, int targetY, int towerX, int towerY, Field& field) override;
    int cast_spell(int, int, int, int, Field&, Player&) override {
        return 0;
    }
    
    int getCost() const override { return cost; }
    int getRange() const override { return range; }
    std::string getName() const override { return name; }
    int getPower() const { return power; }
    virtual void upgrade(int increment = constants::SPELL_DAMAGE_INCREMENT) {
        power += increment;
    }
};
