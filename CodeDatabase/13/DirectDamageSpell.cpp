#include "DirectDamageSpell.h"

#include "../basics/GameLogic.h"

void DirectDamageSpell::spellCast(Coordinates cord, GameLogic &glogic) {
    cord.layer_ = 1;
    Object *target = glogic.getMap().getFromCell(cord);
    Entity *entityPtr = dynamic_cast<Entity *>(target);
    if (entityPtr) {
        entityPtr->onHit(this->damage_);
    }
}

void DirectDamageSpell::setLevel(int level) {
    this->level_ = level;
    this->range_ = this->base_range_ + level;
}
