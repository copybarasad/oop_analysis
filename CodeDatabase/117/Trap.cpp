#include "Trap.h"

Trap::Trap(int damage): Character(TRAP_HEALTH, CharacterCondition::Normal), damage(damage) {}

Trap::Trap(): Trap(DEFAULT_TRAP_DAMAGE) {}

void Trap::activate(Character& target){
    target.takeDMG(this->damage);
    this->takeDMG(this->getMaxHP());
}

int Trap::getDMG(){
    return this->damage;
}

void Trap::setDMG(int damage){
    this->damage = damage;
}

std::string Trap::getName(){
    return "Trap";
}