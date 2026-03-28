#include "Diver.h"
using namespace std;


Diver::Diver(GameConfig &cfg) : config(cfg), hp(cfg.health), damage(cfg.common_damage) {}

int Diver::getHp() {
    return hp;
}

void Diver::setHp(int amount){
    hp = amount;
}

Position &Diver::getPosition() {
    return pos; 
}

void Diver::setPosition(Position &new_pos) {
    pos = new_pos;
}

int Diver::getDamage() {
    return damage;
}

void Diver::setDamage(int amount){
    damage = amount;
}

bool Diver::isAlive() const {
    return hp > 0;
}

void Diver::takeDamage(int amount) {
    hp = max(0, hp - amount);
}

bool Diver::MoveDiver(Position &new_pos){
    Position curr_pos = getPosition();

    int dx = abs(new_pos.x - curr_pos.x);
    int dy = abs(new_pos.y - curr_pos.y);

    if(dx <= 1 && dy <= 1 && (dx+dy) > 0){
        setPosition(new_pos);
        return true;
    }
    return false;
}

void Diver::saveTo(SaveFileRAII &out) const {
    config.saveTo(out);
    out.write(hp).write(damage);
    pos.saveTo(out);
}

void Diver::loadFrom(SaveFileRAII &in){
    config.loadFrom(in);
    in.read(hp).read(damage);
    pos.loadFrom(in);
}