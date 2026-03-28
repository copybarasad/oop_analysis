#include "Character.h"

Character::Character(int hp, int damage, std::pair<int, int> pos){
    this->pos_ = pos;
    this->hp_ = hp;
    this->damage_ = damage;
}

Character::~Character(){
}


int Character::hp() const{
    return hp_;
}

int Character::damage() const{
    return damage_;
}

std::pair<int, int> Character::position() const{
    return pos_;
}

void Character::setPosition(std::pair<int, int> p){
    pos_ = p;
}

bool Character::isAlive() {
    return hp_ > 0;
}

void Character::takeDamage(int amount){
    if (amount >= 0){
        hp_ -= amount;
        if (hp_ < 0) hp_ = 0;
    }
}

void Character::heal(int amount){
    if (amount > 0){
        hp_ += amount;
    }
}

void Character::moveBy(std::pair<int, int> del){
    pos_.first = del.first;
    pos_.second = del.second;
}

void Character::setHp(int hp){
    hp_ = hp;
    if(hp_ < 0){
        hp_ = 0;
    }
}