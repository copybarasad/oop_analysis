#include "Octopus.h"


Octopus::Octopus(GameConfig &cfg) : Diver(cfg) {
    hp = cfg.oct_hp;
    damage = cfg.oct_damage;
    range = cfg.oct_range;
}

int Octopus::getRange(){
    return range;
}

void Octopus::setRange(int amount){
    range = amount;
}

void Octopus::saveTo(SaveFileRAII &out) const{
    Diver::saveTo(out);
    out.write(range);
}

void Octopus::loadFrom(SaveFileRAII &in){
    Diver::loadFrom(in);
    in.read(range);
}