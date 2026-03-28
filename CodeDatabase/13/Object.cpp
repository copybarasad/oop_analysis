#include "GameLogic.h"
#include "Object.h"


Object::~Object() {
    if (glogic_) this->glogic_->removeObject(this->getId());
}

std::string Object::getName() {
    return this->name_;
}


size_t Object::getId() const {
    return this->id_;
}

GameLogic *Object::getGameLogic() {
    return this->glogic_;
}

Map *Object::getMap() {
    return this->map_;
}

ObjTypes Object::getType() const {
    return this->type_;
}


void Object::setId(const size_t new_id) {
    this->id_ = new_id;
}

void Object::setGameLogic(GameLogic *glogic) {
    this->glogic_ = glogic;
    this->map_ = &glogic->getMap();
}


Coordinates Object::getPosition() const {
    return this->position_;
}

void Object::setPosition(const Coordinates &pos) {
    this->position_ = pos;
}

void Object::saveToJson(nlohmann::json *savefile) {
    if (!savefile) return;

    Coordinates pos = this->getPosition();
    (*savefile)["name"] = name_;
    (*savefile)["x"] = pos.x_;
    (*savefile)["y"] = pos.y_;
    (*savefile)["layer"] = pos.layer_;
}
