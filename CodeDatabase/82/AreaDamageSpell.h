#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "Spell.h"

class AreaDamageSpell : public Spell {
private:
    std::string name;
    std::string description;
    int damage;
    int manaCost;
    int range;
    int areaSize;

public:
    AreaDamageSpell(std::string spellName, int spellDamage, int spellManaCost, int spellRange, int spellAreaSize = 2);

    bool cast(const Position& target, GameState& gameState, const Player& caster) override;
    std::string getName() const override {
        return name;
    }
    int getManaCost() const override {
        return manaCost;
    }
    int getRange() const override { 
        return range; 
    }
    bool requiresTarget() const override {
        return true;
    }
    int getDamage() const {
        return damage;
    }
    int getAreaSize() const {
        return areaSize;
    }

private:
    void damageEnemiesInArea(const Position& center, GameState& gameState);
};

#endif
