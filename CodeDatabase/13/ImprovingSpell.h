#ifndef GAME_IMPROVINGSPELL_H
#define GAME_IMPROVINGSPELL_H


#include "Spell.h"

class ImprovingSpell : public Spell {
public:
    ImprovingSpell() : Spell("Improving spell", "Makes your next\n"
                                      "spell stronger",
                              0, 0, 1, ObjTypes::Friendly) {
    };

    void spellCast(Coordinates cord, GameLogic &glogic) override;
};

#endif