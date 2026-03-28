#include "Ally.h"

Ally::Ally(int hp, int damage, std::pair<int,int> pos) : Character(hp, damage, pos){
    skipNextTurn_ = false;
}

bool Ally::shouldSkipTurn(){
    return skipNextTurn_;
}

void Ally::setSkipNextTurn(bool v){
    skipNextTurn_ = v;
}