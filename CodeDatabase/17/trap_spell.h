#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "i_trap_spell.h"

class TrapSpell : public ITrapSpell {
private:
    int damage;
    int range;
public:
    TrapSpell(int trapDamage = 10, int trapRange = 3) 
        : damage(trapDamage), range(trapRange) {}
    
    bool cast(GameMap& map, Position casterPos, Position target) override;
    std::string getName() const override { return "Ловушка"; }
    int getRange(int lev) const override { return range; }
};

#endif