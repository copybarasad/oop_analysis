#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "Spell.h"

class TrapSpell : public Spell {
    int damage;
    int range;
    std::string name;
    
public:
    TrapSpell(int dmg = 20, int rng = 3, std::string spellName = "Trap");
    
    std::string getName() const override;
    std::string getDescription() const override;
    int getRange() const override;
    
    bool canCast(const Position& casterPos, const Position& targetPos, const GameField& field) const override;
    void cast(const Position& casterPos, const Position& targetPos, GameField& field, 
              std::vector<std::shared_ptr<GameObject>>& objects) override;
    
    std::string serialize() const override;
};

#endif