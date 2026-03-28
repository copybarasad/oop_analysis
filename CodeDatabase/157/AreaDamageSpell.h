#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "ISpell.h"

class AreaDamageSpell : public ISpell {
public:
    AreaDamageSpell(int damage, int range);

    // Метод использования заклинания
    bool use(Player& player, GameField& field, int targetX, int targetY) override;

    const char* name() const override { return "AreaDamage"; }

private:
    int damage_;
    int range_;
};

#endif // AREADAMAGESPELL_H