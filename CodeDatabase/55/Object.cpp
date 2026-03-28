#include "Object.h"


Object::Object(GameConfig &cfg) : config(cfg) {}

Position &Object::getPosition() {
    return pos;
}

void Object::setPosition(Position &new_pos) {
    pos = new_pos;
}

void Object::saveTo(SaveFileRAII &out) const{
    config.saveTo(out);
    pos.saveTo(out);
}

void Object::loadFrom(SaveFileRAII &in){
    config.loadFrom(in);
    pos.loadFrom(in);
}