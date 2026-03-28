#include "EnemyTower.h"

EnemyTower::EnemyTower(int maxHealth, int damage, int range): damage(damage), range(range), Character(maxHealth, CharacterCondition::Normal){}

EnemyTower::EnemyTower(): EnemyTower(DEFAULT_MAX_HEALTH, DEFAULT_TOWER_DAMAGE, DEFAULT_TOWER_RANGE) {}

int EnemyTower::getDMG(){
    return this->damage;
}

void EnemyTower::setDMG(int damage){
    this->damage = damage;
}

int EnemyTower::getRange(){
    return this->range;
}

void EnemyTower::setRange(int range){
    this->range = range;
}

std::string EnemyTower::getName(){
    return "Enemy Tower";
}