#include "PlayerTrap.h"
#include "../basics/Entity.h"
#include "../basics/Map.h"
#include "../basics/ObjTypes.h"

void PlayerTrap::update() {
    Coordinates ontop_pos = this->getPosition();
    ontop_pos.layer_ = 1;
    Object *ontop = this->getMap()->getFromCell(ontop_pos);
    Entity *entityPtr = dynamic_cast<Entity *>(ontop);
    if (entityPtr && entityPtr->getType() == ObjTypes::Hostile) {
        entityPtr->onHit(this->damage_);
        delete this;
    }
}

void PlayerTrap::saveToJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Object::saveToJson(savefile);

    (*savefile)["damage"] = damage_;
}

void PlayerTrap::loadFromJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Object::loadFromJson(savefile);

    damage_ = savefile->value("damage", damage_);
}
