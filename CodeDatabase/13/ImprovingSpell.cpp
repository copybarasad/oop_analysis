#include "ImprovingSpell.h"

#include "../basics/GameLogic.h"
#include "../Entities/Player.h"

void ImprovingSpell::spellCast(Coordinates cord, GameLogic &glogic) {
    glogic.getPlayer()->getSpellBook()->changeLevel(this->level_ + 1);
}
