#include "Fortress.h"

Fortress::Fortress(int x, int y,int health, int attackRad, int damage, int attackFrequency):Character(x,y,health),attackFrequency(attackFrequency){
    spell = std::make_unique<DirectFightSpell>(0, attackRad, damage);
}

bool Fortress::attackIsReady(){
    if (attackFrequency == moveCounter){moveCounter = 0; return true;}
    return false;
}

void Fortress::updateCounter(){moveCounter += 1;}