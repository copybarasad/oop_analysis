#include "AreaDamageSpell.h"

#include "../basics/GameLogic.h"

void AreaDamageSpell::spellCast(Coordinates cord, GameLogic &glogic) {
    cord.layer_ = 1;
    for (int d = 0; d < this->area_ * this->area_; d++) {
        Coordinates pos = cord;
        pos.x_ += d % this->area_;
        pos.y_ += d / this->area_;
        Object *target = glogic.getMap().getFromCell(pos);
        Entity *entityPtr = dynamic_cast<Entity *>(target);
        if (entityPtr && entityPtr->getType() == ObjTypes::Hostile) {
            entityPtr->onHit(this->damage_ + std::max(0,
                this->base_area_ + this->level_ - (this->range_ * 2 + 1)));
        }
    }
}

void AreaDamageSpell::setLevel(int level) {
    this->level_ = level;
    this->area_ = std::min(this->base_area_ + level, this->range_ * 2 + 1);
}
