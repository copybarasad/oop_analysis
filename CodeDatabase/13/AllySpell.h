#ifndef GAME_ALLYSPELL_H
#define GAME_ALLYSPELL_H


#include "Spell.h"

class AllySpell : public Spell {
public:
    AllySpell() : Spell("Ally spell", "Spawns ally to\n"
                        "fight on your side",
                        0, 1, 5, ObjTypes::Neutral, 1, true) {
    };

    void spellCast(Coordinates cord, GameLogic &glogic) override;
};


#endif
