#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "ispell.h"

class DirectDamageSpell : public ISpell {
public:
    std::string getName() const override;
    DirectDamageSpell(int damage, int range);
    bool apply(int casterId, Field& field, int targetRow, int targetCol) override;
    int getDamage() const;
    int getRange() const;
private:
    int damage_;
    int range_;
};

#endif
