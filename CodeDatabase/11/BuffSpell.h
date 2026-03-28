#ifndef BUFFSPELL_H
#define BUFFSPELL_H

#include "ISpell.h"

class BuffSpell : public ISpell {
public:
    SpellType getType() const override {return SpellType::BUFF;}
    bool cast(Player& caster, GameController& game) override;
    std::string getName() const override;
    std::string getDescription() const override;
};
#endif