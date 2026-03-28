#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "ispell.h"

class TrapSpell : public ISpell {
public:
    std::string getName() const override;
    TrapSpell(int damage, int range);
    bool apply(int casterId, Field& field, int targetRow, int targetCol) override;
private:
    int damage_;
    int range_;
};

#endif
