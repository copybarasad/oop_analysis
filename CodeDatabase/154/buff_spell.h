#ifndef BUFF_SPELL_H
#define BUFF_SPELL_H

#include "ispell.h"

class BuffSpell : public ISpell {
public:
    std::string getName() const override;
    BuffSpell(int extraRange, int extraArea, int extraTrapDamage, int extraSummons);
    bool apply(int casterId, Field& field, int targetRow, int targetCol) override;
private:
    int extraRange_;
    int extraArea_;
    int extraTrapDamage_;
    int extraSummons_;
};

#endif
