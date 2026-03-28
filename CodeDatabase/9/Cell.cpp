#include "Cell.h"

Cell::Cell(){
    passable_ = true;
    slow_ = false;
    symbol_='.';
    trap_ = false;
    trapDamage_ = 0;
}

bool Cell::isPassable(){
    return passable_;
}

void Cell::setPassable(bool value){
    passable_ = value;
}

bool Cell::isSlow(){
    return slow_;
}

void Cell::setSlow(bool v){
    slow_ = v;
}

char Cell::getSymbol(){
    return symbol_;
}

void Cell::setSymbol(char c){
    symbol_ = c;
}

bool Cell::hasTrap(){
    return trap_; 
}

int  Cell::trapDamage(){
    return trapDamage_;
}

void Cell::setTrap(int damage){
    trap_ = true;
    trapDamage_ = damage;
}

void Cell::clearTrap(){
    trap_ = false;
    trapDamage_ = 0;
}