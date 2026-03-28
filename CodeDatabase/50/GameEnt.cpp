#include "GameEnt.h"

GameEnt::GameEnt(double healthObj, double damageObj, int pos_xObj, int pos_yObj, GameEntType typeObj) 
    : health(healthObj),
      damage(damageObj),
      pos_x(pos_xObj),
      pos_y(pos_yObj),
      type(typeObj) {}

void GameEnt::takeDamage(double gotedDamage){
    if (health - gotedDamage<=0){
        health = 0;
    } else {
        health = health - gotedDamage;
    }
    
}

GameEntType GameEnt::getObjType() const {
    return type;
}

bool GameEnt::isAlive() const {
    return health > 0;
}

void GameEnt::setPosition(int newX, int newY){
    pos_x = newX;
    pos_y = newY;
}

double GameEnt::getMeleeDamage() const {
    return damage;
}
