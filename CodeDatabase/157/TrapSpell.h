#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "ISpell.h"

class TrapSpell : public ISpell {
public:
    TrapSpell(int damage, int range);
    bool use(Player& player, GameField& field, int targetX, int targetY) override;
    const char* name() const override { return "Trap"; }
private:
    int damage_;
    int range_;
};

#endif // TRAPSPELL_H
