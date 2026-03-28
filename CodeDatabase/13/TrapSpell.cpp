#include "TrapSpell.h"

#include "../basics/GameLogic.h"
#include "../Environment/PlayerTrap.h"


void TrapSpell::spellCast(Coordinates cord, GameLogic &glogic) {
    cord.layer_ = 0;
    glogic.addObject(new PlayerTrap(this->damage_ + this->level_), cord);
}
