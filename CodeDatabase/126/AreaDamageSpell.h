#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "Spell.h"

class AreaDamageSpell : public Spell {
    int damage;
    int range;
    std::string name;
    
public:
    AreaDamageSpell(int dmg = 20, int rng = 4, std::string spellName = "Area Damage");
    
    std::string getName() const override;
    std::string getDescription() const override;
    int getRange() const override;
    
    bool canCast(const Position& casterPos, const Position& targetPos, const GameField& field) const override;
    void cast(const Position& casterPos, const Position& targetPos, GameField& field, 
              std::vector<std::shared_ptr<GameObject>>& objects) override;
    
    std::string serialize() const override;
};

#endif