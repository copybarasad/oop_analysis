#include "AllySpell.h"

#include "../basics/GameLogic.h"
#include "../Entities/Ally.h"
#include "../Environment/AllySpawner.h"

void AllySpell::spellCast(Coordinates cord, GameLogic &glogic) {
    if (this->level_ == 0) {
        glogic.addObject(new Ally(), cord);
    } else {
        cord.layer_ = 0;
        glogic.addObject(new AllyBase(level_ + 1), cord);
    }
}
