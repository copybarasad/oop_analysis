#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    std::string name;
    int damage;
    int manaCost;
    int range;

public:
    DirectDamageSpell(std::string spellName, int spellDamage, int spellManaCost, int spellRange);

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
    int getDamage() const override {
        return damage;
    }
};

#endif