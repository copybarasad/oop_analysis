#include "Spell.h"

SpellContext::SpellContext(){
    board = 0;
    player = 0;
    enemies = 0;
    base = 0;
    towers = 0;
    allies = 0;
    hasTarget = false;
    targetCell = std::pair<int,int>(0,0);
}

ISpell::~ISpell(){}

bool ISpell::IsPersistent() const{
    return false; 
}
bool ISpell::CanActivateFromKey() const{
    return false; 
}