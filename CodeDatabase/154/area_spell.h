#ifndef AREA_SPELL_H
#define AREA_SPELL_H

#include "ispell.h"

class AreaDamageSpell : public ISpell {
public:
    std::string getName() const override;
    AreaDamageSpell(int damage, int range);
    bool apply(int casterId, Field& field, int targetRow, int targetCol) override;
    int getDamage() const;
    int getRange() const;
private:
    int damage_;
    int range_;
};

#endif
