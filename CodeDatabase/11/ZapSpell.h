#ifndef ZAPSPELL_H
#define ZAPSPELL_H

#include "ISpell.h"

class ZapSpell : public ISpell {
public:
    SpellType getType() const override { return SpellType::ZAP; }
    bool cast(Player& caster, GameController& game) override;
    std::string getName() const override;
    std::string getDescription() const override;

};

#endif