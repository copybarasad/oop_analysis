#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "ispell.h"

class SummonSpell : public ISpell {
public:
    std::string getName() const override;
    SummonSpell(int allyHealth, int allyDamage, int range, int quantity);
    bool apply(int casterId, Field& field, int targetRow, int targetCol) override;
private:
    int allyHealth_;
    int allyDamage_;
    int range_;
    int quantity_;
};

#endif
