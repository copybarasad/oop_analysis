#ifndef GAME_AREADAMAGESPELL_H
#define GAME_AREADAMAGESPELL_H

#include "Spell.h"

class AreaDamageSpell : public Spell {
private:
    const int base_area_;

public:
    AreaDamageSpell() : Spell("Area damage spell", "Do damage to chosen area",
                              2, 3, 3, ObjTypes::Neutral, 2), base_area_(this->area_) {
    };

    void spellCast(Coordinates cord, GameLogic &glogic) override;

    void setLevel(int level) override;
};


#endif
