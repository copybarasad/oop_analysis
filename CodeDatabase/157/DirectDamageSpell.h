#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "ISpell.h"

class DirectDamageSpell : public ISpell {
public:
    DirectDamageSpell(int damage, int range);
    bool use(Player& player, GameField& field, int targetX, int targetY) override;
    const char* name() const override { return "DirectDamage"; }
    int damage() const { return damage_; }
    int range() const { return range_; }
private:
    int damage_;
    int range_;
};

#endif // DIRECTDAMAGESPELL_H
