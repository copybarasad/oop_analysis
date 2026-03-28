#include "pawn.h"
void pawn::setAttackDmg(size_t attackDmg)
{
    this->attackDmg = attackDmg;
}

size_t pawn::getAttackDmg(){
    return this->attackDmg;
}