#ifndef SUMMONSPELL_H
#define SUMMONSPELL_H

#include "ISpell.h"

class SummonSpell : public ISpell {
public:
    SpellType getType() const override {return SpellType::SUMMON;}
    bool cast(Player& caster, GameController& game) override;
    std::string getName() const override;
    std::string getDescription() const override;
};

#endif