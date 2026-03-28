#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "ISpell.h"

class TrapSpell : public ISpell {
public:
    SpellType getType() const override { return SpellType::TRAP; }
    bool cast(Player& caster, GameController& game) override;
    std::string getName() const override;
    std::string getDescription() const override;
    
};

#endif