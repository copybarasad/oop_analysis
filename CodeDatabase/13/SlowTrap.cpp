#include "SlowTrap.h"
#include "../basics/Entity.h"
#include "../basics/Map.h"
#include "../basics/ObjTypes.h"

void SlowTrap::update() {
    Coordinates ontop_pos = this->getPosition();
    ontop_pos.layer_ = 1;
    Object *ontop = this->getMap()->getFromCell(ontop_pos);
    Entity *entityPtr = dynamic_cast<Entity *>(ontop);
    if (entityPtr && entityPtr->getType() == ObjTypes::Friendly) {
        if (!this->activated_) {
            this->activated_ = true;
            entityPtr->addStun();
        }
    } else {
        activated_ = false;
    }
}

void SlowTrap::saveToJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Object::saveToJson(savefile);

    (*savefile)["activated"] = activated_;
}

void SlowTrap::loadFromJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Object::loadFromJson(savefile);

    activated_ = savefile->value("activated", activated_);
}
