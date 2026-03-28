#include "AllySpawner.h"
#include  "../Entities/Ally.h"

#include "../basics/GameLogic.h"
#include "../basics/Map.h"

void AllyBase::spawnAlly() {
    Coordinates pos = this->getPosition();
    pos.layer_ = 1;
    if (!this->getMap()->isOccupied(pos)) {
        this->getGameLogic()->addObject(new Ally(), pos);
        this->amount_ -= 1;
        if (amount_ == 0) {
            delete this;
        }
    }
}


void AllyBase::update() {
    this->spawnAlly();
}

void AllyBase::saveToJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Object::saveToJson(savefile);

    (*savefile)["amount"] = amount_;
}


void AllyBase::loadFromJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Object::loadFromJson(savefile);

    amount_ = savefile->value("amount", amount_);
}
