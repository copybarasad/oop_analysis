#include "Ally.h"

Ally::Ally(int maxHealth, int damage, CharacterCondition condition): Character(maxHealth, condition), damage(damage) {}

Ally::Ally(): Ally(DEFAULT_MAX_HEALTH, DEFAULT_ALLY_DAMAGE, CharacterCondition::Normal) {}

int Ally::getDMG() const{
    return this->damage;
}

void Ally::setDMG(int damage){
    this->damage = damage;
}

std::string Ally::getName(){
    return "Ally";
}