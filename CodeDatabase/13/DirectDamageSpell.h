#ifndef GAME_DIRECTDAMAGESPELL_H
#define GAME_DIRECTDAMAGESPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    int base_range_;
public:
    DirectDamageSpell():Spell("Direct damage spell", "Do damage to chosen enemy",
        4, 2, 2, ObjTypes::Hostile), base_range_(this->range_){};

    void spellCast(Coordinates cord, GameLogic &glogic) override;

    void setLevel(int level) override;
};

#endif
