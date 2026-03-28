#ifndef GAME_TRAPSPELL_H
#define GAME_TRAPSPELL_H


#include "Spell.h"

class TrapSpell : public Spell {
public:
    TrapSpell() : Spell("Trap spell", "Places a trap that deals\n"
                        "damage to enemy once",
                        2, 4, 1, ObjTypes::Neutral, 1, true) {
    };

    void spellCast(Coordinates cord, GameLogic &glogic) override;
};


#endif
