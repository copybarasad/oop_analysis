#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
    int damage;
    int range;
    std::string name;
    
public:
    DirectDamageSpell(int dmg = 30, int rng = 5, std::string spellName = "Direct Damage");
    
    std::string getName() const override;
    std::string getDescription() const override;
    int getRange() const override;
    
    bool canCast(const Position& casterPos, const Position& targetPos, const GameField& field) const override;
    void cast(const Position& casterPos, const Position& targetPos, GameField& field, 
              std::vector<std::shared_ptr<GameObject>>& objects) override;
    
    std::string serialize() const override;
};

#endif